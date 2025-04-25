#pragma once

#include <functional>
#include <netdb.h>
#include <arpa/inet.h>
#include "esp_netif.h"
#include <esp_timer.h>
#include "esp_log.h"
#include "../constants.h"

namespace pizda {
	enum class TCPState : uint8_t {
		disconnected,
		connecting,
		connected
	};

	class TCP {
		public:
			// -------------------------------- State / connection --------------------------------

			TCPState getState() const {
				return _state;
			}

			void setOnStateChanged(const std::function<void(TCPState, TCPState)>& onStateChanged) {
				_onStateChanged = onStateChanged;
			}

			void connect(const char* address, const uint16_t port) {
				if (_state == TCPState::connected)
					return;

				ESP_LOGI(_loggingTag, "Creating socket");

				inet_pton(AF_INET, address, &_socketAddress.sin_addr);
				_socketAddress.sin_family = AF_INET;
				_socketAddress.sin_port = htons(port);

				_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

				if (_socket < 0) {
					logErrorWithErrno("Unable to create socket");
					return;
				}

				if (fcntl(_socket, F_SETFL, fcntl(_socket, F_GETFL) | O_NONBLOCK) < 0) {
					logErrorWithErrno("Unable to set socket non blocking");
					return;
				}

				ESP_LOGI(_loggingTag, "Connecting");

				setState(TCPState::connecting);
			}

			void disconnect() {
				if (_state == TCPState::disconnected)
					return;

				ESP_LOGI(_loggingTag, "Disconnected");

				::close(_socket);
				_socket = -1;

				clearSendingData();
				_sendingBufferRemaining = -1;

				clearReceivingData();
				_receivingBufferRemaining = -1;

				setState(TCPState::disconnected);
			}

			void scheduleConnection() {
				ESP_LOGI(_loggingTag, "Scheduling reconnection in %lu ms", constants::transceiver::tcp::connectionInterval / 1000);

				_scheduledConnectionTime = esp_timer_get_time() + constants::wifi::connectionInterval;
			}

			void tick() {
				if (_scheduledConnectionTime > 0 && esp_timer_get_time() >= _scheduledConnectionTime) {
					ESP_LOGI(_loggingTag, "Scheduled connection time reached");

					_scheduledConnectionTime = 0;

					connect(
						constants::transceiver::tcp::address,
						constants::transceiver::tcp::port
					);
				}

				switch (_state) {
					case TCPState::connecting:
						connect();
						break;

					case TCPState::connected: {
						if (_sendingBuffer && _sendingBufferRemaining > 0)
							send();

						if (_receivingBuffer && _receivingBufferRemaining > 0)
							receive();

						break;
					}
					default:
						break;
				}
			}

			// -------------------------------- Sending --------------------------------

			void setSendingBuffer(uint8_t* buffer, ssize_t length) {
				_sendingBuffer = buffer;
				_sendingBufferLength = length;
				_sendingBufferRemaining = _sendingBufferLength;
			}

			void setOnSendingCompleted(const std::function<void()>& onSendingCompleted) {
				_onSendingCompleted = onSendingCompleted;
			}
			
			// -------------------------------- Receiving --------------------------------

			void setReceivingBuffer(uint8_t* buffer, ssize_t length) {
				_receivingBuffer = buffer;
				_receivingBufferLength = length;
				_receivingBufferRemaining = _receivingBufferLength;
			}

			void setOnReceivingCompleted(const std::function<void()>& onReceivingCompleted) {
				_onReceivingCompleted = onReceivingCompleted;
			}

		private:
			constexpr static const char* _loggingTag = "TCP";

			struct sockaddr_in _socketAddress;
			int _socket = -1;

			TCPState _state = TCPState::disconnected;

			uint32_t _scheduledConnectionTime = 0;

			uint8_t* _sendingBuffer = nullptr;
			ssize_t _sendingBufferLength = -1;
			ssize_t _sendingBufferRemaining = -1;

			uint8_t* _receivingBuffer = nullptr;
			ssize_t _receivingBufferLength = -1;
			ssize_t _receivingBufferRemaining = -1;

			std::function<void(TCPState, TCPState)> _onStateChanged {};
			std::function<void()> _onSendingCompleted {};
			std::function<void()> _onReceivingCompleted {};

			void logError(const char* text) {
				ESP_LOGE(_loggingTag, "%s", text);
			}

			void logErrorWithErrno(const char* text, int errorCode) {
				ESP_LOGE(_loggingTag, "%s: %s, error code is %d", text, strerror(errorCode), errorCode);
			}

			void logErrorWithErrno(const char* text) {
				logErrorWithErrno(text, errno);
			}

			void clearSendingData() {
				_sendingBuffer = nullptr;
				_sendingBufferLength = -1;
			}

			void clearReceivingData() {
				_receivingBuffer = nullptr;
				_receivingBufferLength = -1;
			}

			void setState(TCPState value) {
				if (value == _state)
					return;

				const auto fromState = _state;
				_state = value;

				_onStateChanged(fromState, _state);
			}

			void connect() {
				// Non-blocking connect() immediately tests for connection status.
				// Here's some remarks about commonly returned values:
				//
				// EINPROGRESS (119) - not an error, meaning connection couldn't be completed right now,
				// because of data layers initialization, crypto, etc. I.e. you have to wait a bit
				// like ~10 ms and test for connect() again
				//
				// EALREADY (120) - not an error, meaning client side was initialized, but server
				// side didn't respond yet. Usually first call to connect() reserves some significant
				// time like 10s and any subsequent call will return EALREADY if server didn't respond.
				// After that time, the ENOTCONN (127) will be returned
				//
				// EISCONN (127) - not an error, meaning connection was successfully established,
				// and you can safely transfer your data via send() / recv()
				//
				// ENOTCONN (128) - error, meaning server didn't respond even for some significant time after
				// connection initialization. You should close socket, re-initialize it and connect again
				if (::connect(_socket, (struct sockaddr*) &_socketAddress, sizeof(_socketAddress)) != 0) {
					// Already connected
					if (errno == EISCONN) {
						ESP_LOGI(_loggingTag, "Connected");

						socklen_t sockLen = (socklen_t) sizeof(int);
						int sockError;

						if (getsockopt(_socket, SOL_SOCKET, SO_ERROR, (void*) &sockError, &sockLen) < 0) {
							logErrorWithErrno("getsockopt() error");
							disconnect();
							return;
						}

						if (sockError) {
							logErrorWithErrno("Connection error", sockError);
							disconnect();
							return;
						}

						setState(TCPState::connected);
					}
					// Some error
					else if (errno != EINPROGRESS && errno != EALREADY) {
						logErrorWithErrno("Unable to connect");
						disconnect();
						return;
					}
				}
			}

			void send() {
				const auto written = ::send(_socket, _sendingBuffer + (_sendingBufferLength - _sendingBufferRemaining), _sendingBufferRemaining, 0);

				if (written < 0 && errno != EINPROGRESS && errno != EAGAIN && errno != EWOULDBLOCK) {
					logErrorWithErrno("send() error");
					disconnect();
					return;
				}

				_sendingBufferRemaining -= written;

				// Finished
				if (_sendingBufferRemaining <= 0) {
					_sendingBufferRemaining = 0;

					clearSendingData();

					_onSendingCompleted();
				}
			}

			void receive() {
				const auto received = recv(_socket, _receivingBuffer + (_receivingBufferLength - _receivingBufferRemaining), _receivingBufferRemaining, 0);

				// Error
				if (received < 0 && errno != EINPROGRESS && errno != EAGAIN && errno != EWOULDBLOCK) {
					logErrorWithErrno("receive() error");
					disconnect();
					return;
				}
				// Received something, maybe just a part of expected data
				else if (received > 0) {
					_receivingBufferRemaining -= received;

					// Receiving completed
					if (_receivingBufferRemaining <= 0) {
						_receivingBufferRemaining = 0;

						clearReceivingData();

						_onReceivingCompleted();
					}
				}
			}
	};
}
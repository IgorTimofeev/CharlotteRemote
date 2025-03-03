#pragma once

#include <netdb.h>
#include <arpa/inet.h>
#include "esp_netif.h"

namespace pizda {
	class TCPClient {
		public:
			TCPClient(const char* address, const uint16_t port) : _address(address), _port(port) {

			}

			// -------------------------------- Connection --------------------------------

			void connect() {
				if (_socketConnected)
					return;

				if (_socket < 0) {
					ESP_LOGI(_loggingTag, "Creating socket");

					inet_pton(AF_INET, _address, &_socketAddress.sin_addr);
					_socketAddress.sin_family = AF_INET;
					_socketAddress.sin_port = htons(_port);

					_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

					if (_socket < 0) {
						logErrorWithErrno("Unable to create socket");
						return;
					}

					if (fcntl(_socket, F_SETFL, fcntl(_socket, F_GETFL) | O_NONBLOCK) < 0) {
						logErrorWithErrno("Unable to set socket non blocking");
						return;
					}
				}

				ESP_LOGI(_loggingTag, "Connecting");

				if (::connect(_socket, (struct sockaddr*) &_socketAddress, sizeof(_socketAddress)) != 0) {
					if (errno == EINPROGRESS) {
						ESP_LOGI(_loggingTag, "Connection in progress");

						fd_set fdset;
						FD_ZERO(&fdset);
						FD_SET(_socket, &fdset);

						// Connection in progress -> have to wait until the connecting socket is marked as writable, i.e. connection completes
						const auto selectResult = select(_socket + 1, NULL, &fdset, NULL, NULL);

						if (selectResult < 0) {
							logErrorWithErrno("Error during connection, select for socket to be writable");
							close();
							return;
						}
						else if (selectResult == 0) {
							logErrorWithErrno("Connection timeout, select for socket to be writable");
							close();
							return;
						}
						else {
							socklen_t sockLen = (socklen_t) sizeof(int);
							int sockError;

							if (getsockopt(_socket, SOL_SOCKET, SO_ERROR, (void*) &sockError, &sockLen) < 0) {
								logErrorWithErrno("Error when getting socket error using getsockopt()");
								close();
								return;
							}

							if (sockError) {
								logErrorWithErrno("Connection error: %d, %s", sockError);
								close();
								return;
							}
						}
					}
					else {
						logErrorWithErrno("Unable to connect");
						close();
						return;
					}
				}

				ESP_LOGI(_loggingTag, "Connected");

				_socketConnected = true;
			}

			bool isConnected() {
				return _socketConnected;
			}

			void disconnect() {
				if (!_socketConnected)
					return;

				close();
				_socketConnected = false;

				clearSendingData();
				_sendingBufferRemaining = -1;

				clearReceivingData();
				_receivingBufferRemaining = -1;
			}

			// -------------------------------- Sending --------------------------------


			void setSendingBuffer(uint8_t* buffer, ssize_t length) {
				_sendingBuffer = buffer;
				_sendingBufferLength = length;
				_sendingBufferRemaining = _sendingBufferLength;
			}

			bool isReadyToSendNext() {
				return _socketConnected && _sendingBufferRemaining <= 0;
			}

			bool isSendingFinished() {
				return _sendingBufferLength == 0;
			}

			void send() {
				if (!_socketConnected || !_sendingBuffer || _sendingBufferRemaining <= 0)
					return;

				const auto written = ::send(_socket, _sendingBuffer + (_sendingBufferLength - _sendingBufferRemaining), _sendingBufferRemaining, 0);

				if (written < 0 && errno != EINPROGRESS && errno != EAGAIN && errno != EWOULDBLOCK) {
					logErrorWithErrno("Error occurred during sending");
					disconnect();
					return;
				}

				_sendingBufferRemaining -= written;

				// Finished
				if (_sendingBufferRemaining <= 0) {
					_sendingBufferRemaining = 0;

					clearSendingData();
				}
			}

			// -------------------------------- Receiving --------------------------------

			void setReceivingBuffer(uint8_t* buffer, ssize_t length) {
				_receivingBuffer = buffer;
				_receivingBufferLength = length;
				_receivingBufferRemaining = _receivingBufferLength;
			}

			bool isReadyToReceiveNext() {
				return _socketConnected && _receivingBufferRemaining <= 0;
			}

			bool isReceivingFinished() {
				return _receivingBufferRemaining == 0;
			}

			void receive() {
				if (!_socketConnected || !_receivingBuffer || _receivingBufferRemaining <= 0)
					return;

				const auto received = recv(_socket, _receivingBuffer + (_receivingBufferLength - _receivingBufferRemaining), _receivingBufferRemaining, 0);

				if (received < 0) {
					if (errno == EINPROGRESS || errno == EAGAIN || errno == EWOULDBLOCK) {
						// Not an error
						return;
					}
					else if (errno == ENOTCONN) {
						logError("Connection closed");
						disconnect();
						return;
					}
					else {
						logErrorWithErrno("Error occurred during receiving");
						disconnect();
						return;
					}
				}
				// Nothing to read
				else if (received == 0) {

				}
				// Received something (maybe only part)
				else {
					_receivingBufferRemaining -= received;

					// Receiving completed
					if (_receivingBufferRemaining <= 0) {
						_receivingBufferRemaining = 0;

						clearReceivingData();
					}
				}
			}
		private:
			constexpr static const char* _loggingTag = "TCP";

			const char* _address;
			const uint16_t _port;

			struct sockaddr_in _socketAddress;
			int _socket = -1;
			bool _socketConnected = false;

			uint8_t* _sendingBuffer = nullptr;
			ssize_t _sendingBufferLength = -1;
			ssize_t _sendingBufferRemaining = -1;

			uint8_t* _receivingBuffer = nullptr;
			ssize_t _receivingBufferLength = -1;
			ssize_t _receivingBufferRemaining = -1;

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

			void close() {
				::close(_socket);
				_socket = -1;
			}
	};
}
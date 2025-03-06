#include "WiFiTransceiver.h"
#include <esp_timer.h>
#include "../../rc.h"
#include "../../constants.h"
#include "../../resources/sounds.h"

namespace pizda {
	void WiFiTransceiver::setup() {
		_WiFi.setOnStateChanged([this]() {
			switch (_WiFi.getState()) {
				case WiFiState::started: {
					_WiFi.connect();
					break;
				}
				case WiFiState::connected: {
					_TCP.connect(
						constants::transceiver::tcp::address,
						constants::transceiver::tcp::port
					);

					break;
				}
				case WiFiState::disconnected: {
					ESP_LOGI("Transceiver", "WiFi reconnection scheduled for %lu s", constants::wifi::connectionInterval);

					_TCP.disconnect();

					_WiFiConnectTime = esp_timer_get_time() + constants::wifi::connectionInterval;

					break;
				}
				default:
					break;
			}
		});

		_TCP.setOnStateChanged([this]() {
			switch (_TCP.getState()) {
				case TCPState::connected: {
					RC::getInstance().getSpeaker().play(resources::sounds::transceiverConnect());

					setTCPSendingBuffer();
					setTCPReceivingBuffer();

					break;
				}
				case TCPState::disconnected: {
					ESP_LOGI("Transceiver", "TCP reconnection scheduled for %lu s", constants::transceiver::tcp::connectionInterval);

					RC::getInstance().getSpeaker().play(resources::sounds::transceiverDisconnect());

					_TCPConnectTime = esp_timer_get_time() + constants::transceiver::tcp::connectionInterval;

					break;
				}
				default:
					break;
			}
		});

		_TCP.setOnSendingCompleted([this]() {
			setTCPSendingBuffer();
		});

		_TCP.setOnReceivingCompleted([this]() {
			setTCPReceivingBuffer();
		});

		_WiFi.start(
			constants::wifi::ssid,
			constants::wifi::password,
			WIFI_AUTH_WPA2_PSK
		);
	}

	void WiFiTransceiver::tick() {
		if (esp_timer_get_time() < _tickTime)
			return;

		if (_WiFiConnectTime > 0 && esp_timer_get_time() > _WiFiConnectTime) {
			ESP_LOGI("Transceiver", "WiFi reconnection time reached");

			_WiFiConnectTime = 0;

			_WiFi.connect();
		}

		if (_TCPConnectTime > 0 && esp_timer_get_time() > _TCPConnectTime) {
			ESP_LOGI("Transceiver", "TCP reconnection time reached");

			_TCPConnectTime = 0;

			_TCP.connect(
				constants::transceiver::tcp::address,
				constants::transceiver::tcp::port
			);
		}

		_TCP.tick();

		_tickTime = esp_timer_get_time() + constants::transceiver::tickInterval;
	}

	void WiFiTransceiver::fillRemotePacket() {
		auto& rc = RC::getInstance();
		auto& settings = rc.getSettings();

		_remotePacket.throttle1 = settings.controls.throttles[0];
		_remotePacket.throttle2 = settings.controls.throttles[1];

		_remotePacket.ailerons = rc.getJoystickHorizontal().getMappedUint16Value();
		_remotePacket.elevator = rc.getJoystickVertical().getMappedUint16Value();

		_remotePacket.rudder = rc.getRing().getMappedUint16Value();
		_remotePacket.flaps = rc.getLeverRight().getMappedUint16Value();
		_remotePacket.spoilers = rc.getLeverLeft().getMappedUint16Value();

		_remotePacket.landingGear = settings.controls.landingGear;
		_remotePacket.strobeLights = settings.controls.strobeLights;
	}

	void WiFiTransceiver::setTCPSendingBuffer() {
		fillRemotePacket();

		_TCP.setSendingBuffer((uint8_t*) &_remotePacket, sizeof(RemotePacket));
	}

	void WiFiTransceiver::setTCPReceivingBuffer() {
		RC::getInstance().handleAircraftPacket(&_aircraftPacket);

		_TCP.setReceivingBuffer((uint8_t*) &_aircraftPacket, sizeof(AircraftPacket));
	}
}
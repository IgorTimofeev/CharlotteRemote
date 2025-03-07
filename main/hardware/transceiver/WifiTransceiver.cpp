#include "WiFiTransceiver.h"
#include <esp_timer.h>
#include "../../rc.h"
#include "../../resources/sounds.h"

namespace pizda {
	void WiFiTransceiver::setup() {
		_WiFi.setOnStateChanged([this](WiFiState fromState, WiFiState toState) {
			switch (toState) {
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
					_TCP.disconnect();
					_WiFi.scheduleConnection();

					break;
				}
				default:
					break;
			}
		});

		_TCP.setOnStateChanged([this](TCPState fromState, TCPState toState) {
			switch (toState) {
				case TCPState::connected: {
					RC::getInstance().getSpeaker().play(resources::sounds::transceiverConnect());

					setTCPSendingBuffer();
					setTCPReceivingBuffer();

					break;
				}
				case TCPState::disconnected: {
					if (fromState == TCPState::connected)
						RC::getInstance().getSpeaker().play(resources::sounds::transceiverDisconnect());

					_TCP.scheduleConnection();

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

		_WiFi.tick();
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
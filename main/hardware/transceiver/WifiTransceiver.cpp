#include "WiFiTransceiver.h"
#include <esp_timer.h>
#include "../../constants.h"
#include "../../rc.h"

namespace pizda {
	void WiFiTransceiver::setup() {
		_WiFi.setup(
			constants::wifi::ssid,
			constants::wifi::password,
			WIFI_AUTH_WPA2_PSK
		);
	}

	void WiFiTransceiver::tick() {
		if (esp_timer_get_time() < _tickTime)
			return;

		switch (_WiFi.getState()) {
			case WiFiState::readyToConnect:
			case WiFiState::disconnected: {
				if (esp_timer_get_time() > _WiFiConnectTime) {
					_WiFi.connect();

					_WiFiConnectTime = esp_timer_get_time() + constants::wifi::connectionInterval;
				}

				break;
			}
			case WiFiState::connected:
				switch (_TCP.getState()) {
					case TCPState::disconnected: {
						if (esp_timer_get_time() > _TCPConnectTime) {
							_TCP.connect(
								constants::transceiver::tcp::address,
								constants::transceiver::tcp::port
							);

							_TCPConnectTime = esp_timer_get_time() + constants::transceiver::tcp::connectionInterval;
						}

						break;
					}
					case TCPState::connecting:
						break;

					case TCPState::connected: {
						_TCP.send();

						if (_TCP.isReadyToSendNext()) {
							fillRemotePacket();

							_TCP.setSendingBuffer((uint8_t*) &_remotePacket, sizeof(RemotePacket));
						}

						_TCP.receive();

						if (_TCP.isReadyToReceiveNext()) {
							if (_TCP.isReceivingFinished()) {
								RC::getInstance().handleAircraftPacket(&_aircraftPacket);
							}

							_TCP.setReceivingBuffer((uint8_t*) &_aircraftPacket, sizeof(AircraftPacket));
						}

						break;
					}
				}

				break;

			default:
				break;
		}

		_tickTime = esp_timer_get_time() + constants::transceiver::tickInterval;
	}

	void WiFiTransceiver::fillRemotePacket() {
		auto& rc = RC::getInstance();

		_remotePacket.throttle1 = rc.getThrottles()[0];
		_remotePacket.throttle2 = rc.getThrottles()[1];

		_remotePacket.ailerons = rc.getJoystickHorizontal().getMappedUint16Value();
		_remotePacket.elevator = rc.getJoystickVertical().getMappedUint16Value();

		_remotePacket.rudder = rc.getRing().getMappedUint16Value();
		_remotePacket.flaps = rc.getLeverRight().getMappedUint16Value();
		_remotePacket.spoilers = rc.getLeverLeft().getMappedUint16Value();

		_remotePacket.landingGear = rc.getLandingGear();
		_remotePacket.strobeLights = rc.getStrobeLights();
	}
}
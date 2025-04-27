#include "TCPTransceiver.h"
#include <esp_timer.h>
#include "../../rc.h"
#include "../../resources/sounds.h"

namespace pizda {
	void TCPTransceiver::setup() {
		WiFi::addOnStateChanged([this](WiFiState fromState, WiFiState toState) {
			switch (toState) {
				case WiFiState::connected: {
					_TCP.connect(
						constants::transceiver::tcp::address,
						constants::transceiver::tcp::port
					);

					break;
				}
				case WiFiState::disconnected: {
					_TCP.disconnect();

					break;
				}
				default:
					break;
			}
		});

		_TCP.setOnStateChanged([this](TCPState fromState, TCPState toState) {
			switch (toState) {
				case TCPState::connected: {
					RC::getInstance().getSpeaker().play(resources::Sounds::transceiverConnect());

					fillRemotePacket();
					setTCPSendingBuffer();

					setTCPReceivingBuffer();

					break;
				}
				case TCPState::disconnected: {
					if (fromState == TCPState::connected)
						RC::getInstance().getSpeaker().play(resources::Sounds::transceiverDisconnect());

					_TCP.scheduleConnection();

					break;
				}
				default:
					break;
			}
		});

		_TCP.setOnSendingCompleted([this]() {
			fillRemotePacket();
			setTCPSendingBuffer();
		});

		_TCP.setOnReceivingCompleted([this]() {
			RC::getInstance().handleAircraftPacket(&_aircraftPacket);
			setTCPReceivingBuffer();
		});

		WiFi::start();
	}

	void TCPTransceiver::tick() {
		if (esp_timer_get_time() < _tickTime)
			return;

		_TCP.tick();

		_tickTime = esp_timer_get_time() + constants::transceiver::tickInterval;
	}

	void TCPTransceiver::fillRemotePacket() {
		auto& rc = RC::getInstance();
		auto& settings = rc.getSettings();

		_remotePacket.throttle1 = settings.controls.throttles[0];
		_remotePacket.throttle2 = settings.controls.throttles[1];

		_remotePacket.ailerons = rc.getJoystickHorizontal().getMappedUint16Value();
		_remotePacket.elevator = rc.getJoystickVertical().getMappedUint16Value();

		_remotePacket.rudder = rc.getRing().getMappedUint16Value();
		_remotePacket.flaps = rc.getLeverRight().getMappedUint16Value();
		_remotePacket.spoilers = rc.getLeverLeft().getMappedUint16Value();

		_remotePacket.altimeterPressurePa = settings.controls.referencePressureSTD ? 101325 : settings.controls.referencePressurePa;

		_remotePacket.autopilotAirspeedMs = convertSpeed(settings.autopilot.speedKt, SpeedUnit::knot, SpeedUnit::meterPerSecond);
		_remotePacket.autopilotAutoThrottle = settings.autopilot.autoThrottle;

		_remotePacket.autopilotHeadingDeg = settings.autopilot.headingDeg;
		_remotePacket.autopilotHeadingHold = settings.autopilot.headingHold;

		_remotePacket.autopilotAltitudeM = convertDistance(settings.autopilot.altitudeFt, DistanceUnit::foot, DistanceUnit::meter);
		_remotePacket.autopilotLevelChange = settings.autopilot.levelChange;

		_remotePacket.landingGear = settings.controls.landingGear;
		_remotePacket.strobeLights = settings.controls.strobeLights;
	}

	void TCPTransceiver::setTCPSendingBuffer() {
		_TCP.setSendingBuffer((uint8_t*) &_remotePacket, sizeof(RemotePacket));
	}

	void TCPTransceiver::setTCPReceivingBuffer() {
		_TCP.setReceivingBuffer((uint8_t*) &_aircraftPacket, sizeof(AircraftPacket));
	}
}
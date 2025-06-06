#include "TCPTransceiver.h"
#include <esp_timer.h>
#include <rc.h>
#include <resources/sounds.h>

namespace pizda {
	void TCPTransceiver::setup() {
		WiFi::isConnectedChanged += [this] {
			if (WiFi::isConnected()) {
				_TCP.connect(
					constants::transceiver::tcp::address,
					constants::transceiver::tcp::port
				);
			}
			else {
				_TCP.disconnect();
			}
		};

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

					if (WiFi::isConnected())
						_TCP.scheduleConnection();

					break;
				}
				default:
					break;
			}
		});

		_TCP.setOnSendingCompleted([this] {
			fillRemotePacket();
			setTCPSendingBuffer();
		});

		_TCP.setOnReceivingCompleted([this] {
			RC::getInstance().handleAircraftPacket(&_aircraftPacket);
			setTCPReceivingBuffer();
		});
	}

	bool TCPTransceiver::isStarted() const {
		return _startTask != nullptr;
	}

	void TCPTransceiver::start() {
		if (isStarted())
			return;

		xTaskCreate(startTaskFunction, "Transceiver start", 4096, this, 1, _startTask);
	}

	void TCPTransceiver::startTaskFunction(void* arg) {
		const auto instance = static_cast<TCPTransceiver*>(arg);

		while (true) {
			instance->_TCP.tick();

			vTaskDelay(pdMS_TO_TICKS(constants::transceiver::tickInterval / 1000));
		}
	}

	void TCPTransceiver::stop() {
		if (!isStarted())
			return;

		ESP_LOGE("Transceiver", "Stopped");

		vTaskDelete(*_startTask);
		_startTask = nullptr;
	}

	void TCPTransceiver::fillRemotePacket() {
		auto& rc = RC::getInstance();
		const auto& settings = rc.getSettings();

		_remotePacket.throttle = settings.controls.throttle;

		_remotePacket.ailerons = rc.getJoystickHorizontal().getMappedUint8Value();
		_remotePacket.elevator = rc.getJoystickVertical().getMappedUint8Value();

		_remotePacket.rudder = rc.getRing().getMappedUint8Value();
		_remotePacket.flaps = rc.getLeverRight().getMappedUint8Value();
		_remotePacket.spoilers = rc.getLeverLeft().getMappedUint8Value();

		_remotePacket.altimeterPressurePa = settings.controls.referencePressureSTD ? 101325 : settings.controls.referencePressurePa;

		_remotePacket.autopilotAirspeedMs = Units::convertSpeed(settings.autopilot.speedKt, SpeedUnit::knot, SpeedUnit::meterPerSecond);
		_remotePacket.autopilotAutoThrottle = settings.autopilot.autoThrottle;

		_remotePacket.autopilotHeadingDeg = settings.autopilot.headingDeg;
		_remotePacket.autopilotHeadingHold = settings.autopilot.headingHold;

		_remotePacket.autopilotAltitudeM = Units::convertDistance(settings.autopilot.altitudeFt, DistanceUnit::foot, DistanceUnit::meter);
		_remotePacket.autopilotLevelChange = settings.autopilot.levelChange;

		_remotePacket.landingGear = settings.controls.landingGear;
		_remotePacket.strobeLights = settings.controls.strobeLights;
	}

	void TCPTransceiver::setTCPSendingBuffer() {
		_TCP.setSendingBuffer(reinterpret_cast<uint8_t*>(&_remotePacket), sizeof(RemotePacket));
	}

	void TCPTransceiver::setTCPReceivingBuffer() {
		_TCP.setReceivingBuffer(reinterpret_cast<uint8_t*>(&_aircraftPacket), sizeof(AircraftPacket));
	}
}
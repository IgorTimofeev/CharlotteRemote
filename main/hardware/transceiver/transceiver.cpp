#include "transceiver.h"

#include "rc.h"
#include "config.h"

namespace pizda {
	void Transceiver::setup() {
		const auto sxSetupValid = sx1262.setup(
			config::spi::host,
			config::transceiver::SPIFrequencyHz,
			
			config::transceiver::slaveSelect,
			config::transceiver::busy,
			config::transceiver::reset,
			config::transceiver::DIO1,
			
			config::transceiver::RFFrequencyMHz,
			config::transceiver::bandwidthKHz,
			config::transceiver::spreadingFactor,
			config::transceiver::codingRate,
			config::transceiver::syncWord,
			config::transceiver::powerDBm,
			config::transceiver::preambleLength
		);
		
		if (!sxSetupValid) {
			ESP_LOGE(_logTag, "SX1262 setup failed");
		}
	}
	
	void Transceiver::start() {
		if (_started)
			return;
		
		_started = true;
		
		xTaskCreate(
			[](void* arg) {
				static_cast<Transceiver*>(arg)->onStart();
			},
			"Transceiver",
			4096,
			this,
			configMAX_PRIORITIES - 1,
			nullptr
		);
	}
	
	void Transceiver::stop() {
		if (!_started)
			return;
		
		ESP_LOGE("Transceiver", "Stopped");

	}
	
	void Transceiver::fillRemotePacket() {
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
	
	void Transceiver::onStart() {
		
		while (true) {
			const uint8_t pizda[] {
				0xAA, 0xBB, 0xCC
			};
			
			const auto state = sx1262.transmit(pizda, 3);
			
			ESP_LOGI(_logTag, "transmit state: %d", state);
			
			vTaskDelay(pdMS_TO_TICKS(1'000));
		}
	}
}
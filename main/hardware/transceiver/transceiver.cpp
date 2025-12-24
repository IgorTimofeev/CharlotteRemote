#include "transceiver.h"

#include <esp_timer.h>
#include <esp_log.h>

#include "rc.h"
#include "config.h"

namespace pizda {
	void Transceiver::setup() {
		_setupValid = sx1262.setup(
			config::spi::host,
			config::transceiver::SPIFrequencyHz,
			
			config::transceiver::slaveSelect,
			config::transceiver::reset,
			config::transceiver::busy,
			config::transceiver::DIO1,
			
			config::transceiver::RFFrequencyMHz,
			config::transceiver::bandwidthKHz,
			config::transceiver::spreadingFactor,
			config::transceiver::codingRate,
			config::transceiver::syncWord,
			config::transceiver::powerDBm,
			config::transceiver::preambleLength
		);
		
		if (!_setupValid) {
			ESP_LOGE(_logTag, "SX1262 setup failed");
		}
	}
	
	float Transceiver::getRSSI() {
		return _RSSI;
	}
	
	void Transceiver::start() {
		if (!_setupValid || _started)
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
		if (!_setupValid || !_started)
			return;
		
		ESP_LOGI(_logTag, "Stopped");
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
		ESP_LOGI(_logTag, "Started");
		
		while (true) {
			constexpr static uint8_t bufferLength = 255;
			uint8_t buffer[bufferLength];
			std::memset(buffer, 0, bufferLength);
			uint8_t receivedLength = 0;
			
			if (!sx1262.receive(buffer, receivedLength, 1'000'000))
				continue;
			
//			ESP_LOGI(_logTag, "receivedLength: %d", receivedLength);
//
//			for (int i = 0; i < receivedLength; ++i) {
//				ESP_LOGI(_logTag, "buffer[%d]: %d", i, buffer[i]);
//			}
//
			// Header validation
			if (std::memcmp(reinterpret_cast<const uint8_t*>(Packet::header), buffer, Packet::headerLengthBytes) == 0) {
				BitStream stream { buffer + Packet::headerLengthBytes };
				
				auto packetType = static_cast<PacketType>(stream.readUint8(Packet::typeLengthBits));
//				ESP_LOGI(_logTag, "packet type: %d", packetType);

				switch (packetType) {
					case PacketType::AircraftAHRS:
						parseAircraftPacket(stream);
						break;
					default:
						break;
				}
				
				// RSSI
				sx1262.getRSSI(_RSSI);
			}
			else {
				ESP_LOGE(_logTag, "invalid packet header: %s", buffer);
				
				continue;
			}
		}
	}
	
	void Transceiver::parseAircraftPacket(BitStream& stream) {
		auto& rc = RC::getInstance();
		auto& ad = rc.getAircraftData();
		
		const auto time = esp_timer_get_time();
		const auto deltaTime = time - _aircraftPacketTime;
		_aircraftPacketTime = time;
		
		const auto oldSpeed = ad.airSpeedKt;
		const auto oldAltitude = ad.altitudeFt;
		
		// Direct reading
		ad.rollRad = stream.readFloat();
		ad.pitchRad = stream.readFloat();
		ad.yawRad = stream.readFloat();
		ad.airSpeedKt = stream.readFloat();
		ad.altitudeFt = stream.readFloat();
//		ad.throttle = packet->throttle;
		
		
		// Value conversions
//
//		ad.geographicCoordinates.setLatitude(packet->latitudeRad);
//		ad.geographicCoordinates.setLongitude(packet->longitudeRad);
//		ad.geographicCoordinates.setAltitude(packet->altitudeM);
//
//		ad.windSpeed = Units::convertSpeed(packet->windSpeedMs, SpeedUnit::meterPerSecond, SpeedUnit::knot);
		
		ad.airSpeedKt = Units::convertSpeed(ad.airSpeedKt, SpeedUnit::meterPerSecond, SpeedUnit::knot);
		ad.altitudeFt = Units::convertDistance(ad.altitudeFt, DistanceUnit::meter, DistanceUnit::foot);
//		ad.groundSpeedKt = Units::convertSpeed(packet->groundSpeedMs, SpeedUnit::meterPerSecond, SpeedUnit::knot);
		
//		ad.flightPathVectorPitch = packet->flightPathPitchRad;
//		ad.flightPathVectorYaw = packet->flightPathYawRad;
//
//		ad.flightDirectorPitch = packet->flightDirectorPitchRad;
//		ad.flightDirectorRoll = packet->flightDirectorYawRad;
//
//		ad.slipAndSkid = -1.f + static_cast<float>(packet->slipAndSkid) / static_cast<float>(0xFFFF) * 2.f;
//
//		ad.windDirection = toRadians(packet->windDirectionDeg);
		
		// Trends
		const auto deltaAltitude = ad.altitudeFt - oldAltitude;
		
		// Airspeed & altitude, 10 sec
		ad.airSpeedTrend = (ad.airSpeedKt - oldSpeed) * 10'000'000.f / static_cast<float>(deltaTime);
		ad.altitudeTrend = deltaAltitude * 10'000'000.f / static_cast<float>(deltaTime);
		
		// Vertical speed, 1 min
		ad.verticalSpeed = deltaAltitude * 60'000'000.f / static_cast<float>(deltaTime);
	}
}
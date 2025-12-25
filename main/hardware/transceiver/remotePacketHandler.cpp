#include "remotePacketHandler.h"

#include <utility>

#include <esp_timer.h>

#include "rc.h"
#include "resources/sounds.h"

namespace pizda {
	// -------------------------------- Reading --------------------------------
	
	bool RemotePacketHandler::readPacket(BitStream& stream, PacketType packetType, uint8_t payloadLength) {
		switch (packetType) {
			case PacketType::aircraftADIRS: {
				return readAircraftADIRSPacket(stream, payloadLength);
			}
			default: {
				ESP_LOGE(_logTag, "failed to read packet: unsupported type %d", std::to_underlying(packetType));
				
				return false;
			}
		}
	}
	
	bool RemotePacketHandler::readAircraftADIRSPacket(BitStream& stream, uint8_t payloadLength) {
		if (!validatePayloadChecksumAndLength(stream, 5 * 32, payloadLength))
			return false;
		
		auto& rc = RC::getInstance();
		auto& ad = rc.getAircraftData();
		
		const auto time = esp_timer_get_time();
		const auto deltaTime = time - _aircraftADIRSPacketTime;
		_aircraftADIRSPacketTime = time;
		
		const auto oldSpeed = ad.airSpeedKt;
		const auto oldAltitude = ad.altitudeFt;
		
		auto sanitizeFloat = [](float value, float min, float max) {
			if (value < min) {
				value = min;
				
				ESP_LOGW(_logTag, "value %f is out of range [%f, %f]", value, min, max);
			}
			else if (value > max) {
				value = max;
				
				ESP_LOGW(_logTag, "value %f is out of range [%f, %f]", value, min, max);
			}
			
			return value;
		};
		
		// Direct reading
		ad.rollRad = sanitizeFloat(stream.readFloat(), toRadians(-360), toRadians(360));
		ad.pitchRad = sanitizeFloat(stream.readFloat(), toRadians(-360), toRadians(360));
		ad.yawRad = sanitizeFloat(stream.readFloat(), toRadians(-360), toRadians(360));
		
		ad.airSpeedKt = sanitizeFloat(stream.readFloat(), 0, 999);
		ad.altitudeFt = sanitizeFloat(stream.readFloat(), 0, 99999);
		
//		ad.throttle = ...
		
		
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
		
		return true;
	}
	
	// -------------------------------- Writing --------------------------------
	
	bool RemotePacketHandler::writePacket(BitStream& stream, PacketType packetType) {
		switch (packetType) {
			default: {
				ESP_LOGE(_logTag, "failed to write packet: unsupported type %d", std::to_underlying(packetType));
				
				return false;
			}
		}
	}
	
	//	void Transceiver::fillRemotePacket() {
//		auto& rc = RC::getInstance();
//		const auto& settings = rc.getSettings();
//
//		_remotePacket.throttle = settings.controls.throttle;
//
//		_remotePacket.ailerons = rc.getJoystickHorizontal().getMappedUint8Value();
//		_remotePacket.elevator = rc.getJoystickVertical().getMappedUint8Value();
//
//		_remotePacket.rudder = rc.getRing().getMappedUint8Value();
//		_remotePacket.flaps = rc.getLeverRight().getMappedUint8Value();
//		_remotePacket.spoilers = rc.getLeverLeft().getMappedUint8Value();
//
//		_remotePacket.altimeterPressurePa = settings.controls.referencePressureSTD ? 101325 : settings.controls.referencePressurePa;
//
//		_remotePacket.autopilotAirspeedMs = Units::convertSpeed(settings.autopilot.speedKt, SpeedUnit::knot, SpeedUnit::meterPerSecond);
//		_remotePacket.autopilotAutoThrottle = settings.autopilot.autoThrottle;
//
//		_remotePacket.autopilotHeadingDeg = settings.autopilot.headingDeg;
//		_remotePacket.autopilotHeadingHold = settings.autopilot.headingHold;
//
//		_remotePacket.autopilotAltitudeM = Units::convertDistance(settings.autopilot.altitudeFt, DistanceUnit::foot, DistanceUnit::meter);
//		_remotePacket.autopilotLevelChange = settings.autopilot.levelChange;
//
//		_remotePacket.landingGear = settings.controls.landingGear;
//		_remotePacket.strobeLights = settings.controls.strobeLights;
//	}
	
	void RemotePacketHandler::onConnectionLost() {
		auto& rc = RC::getInstance();
		
		rc.getSpeaker().play(resources::Sounds::transceiverDisconnect());
	}
	
	void RemotePacketHandler::onConnectionRestored() {
		auto& rc = RC::getInstance();
		
		rc.getSpeaker().play(resources::Sounds::transceiverConnect());
	}
}
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
		if (!validatePayloadChecksumAndLength(stream, 10 * 3 + 7 + 14, payloadLength))
			return false;
		
		auto& rc = RC::getInstance();
		auto& ad = rc.getAircraftData();
		
		const auto time = esp_timer_get_time();
		const auto deltaTime = time - _aircraftADIRSPacketTime;
		_aircraftADIRSPacketTime = time;
		
		const auto oldSpeed = ad.raw.airSpeedKt;
		const auto oldAltitude = ad.raw.altitudeFt;
		
		// Direct reading
		auto readRadians = [&stream](uint8_t bits) {
			auto value = static_cast<float>(stream.readUint16(bits)) / static_cast<float>(1 << bits);
			value = value - 0.5f;
			value = value * (2 * std::numbers::pi_v<float>);
			
			return sanitizeValue<float>(value, toRadians(-180), toRadians(180));
		};
		
		ad.raw.rollRad = readRadians(10);
		ad.raw.pitchRad = readRadians(10);
		ad.raw.yawRad = readRadians(10);
		
		ad.raw.airSpeedKt = static_cast<float>(sanitizeValue<uint8_t>(stream.readUint8(7), 0, 255));
		
		// Altitude
		constexpr static int16_t altitudeMin = -1'000;
		constexpr static int16_t altitudeMax = 10'000;
		constexpr static uint8_t altitudeBits = 14;
		
		const auto altitudeValue = stream.readUint16(altitudeBits);
		const auto altitudeFactor = static_cast<float>(altitudeValue) / static_cast<float>(1 << altitudeBits);
		ad.raw.altitudeFt = altitudeMin + (altitudeMax - altitudeMin) * altitudeFactor;
		
//		ad.throttle = ...
		
		
		// Value conversions
//
//		ad.raw.geographicCoordinates.setLatitude(packet->latitudeRad);
//		ad.raw.geographicCoordinates.setLongitude(packet->longitudeRad);
//		ad.raw.geographicCoordinates.setAltitude(packet->altitudeM);
//
//		ad.windSpeed = Units::convertSpeed(packet->windSpeedMs, SpeedUnit::meterPerSecond, SpeedUnit::knot);
		
		ad.raw.airSpeedKt = Units::convertSpeed(ad.raw.airSpeedKt, SpeedUnit::meterPerSecond, SpeedUnit::knot);
		ad.raw.altitudeFt = Units::convertDistance(ad.raw.altitudeFt, DistanceUnit::meter, DistanceUnit::foot);
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
		const auto deltaAltitude = ad.raw.altitudeFt - oldAltitude;
		
		// Airspeed & altitude, 5 sec
		ad.raw.airSpeedTrend = (ad.raw.airSpeedKt - oldSpeed) * 5'000'000.f / static_cast<float>(deltaTime);
		ad.raw.altitudeTrend = deltaAltitude * 5'000'000.f / static_cast<float>(deltaTime);
		
		// Vertical speed, 1 min
		ad.raw.verticalSpeed = deltaAltitude * 60'000'000.f / static_cast<float>(deltaTime);
		
		return true;
	}
	
	// -------------------------------- Writing --------------------------------
	
	bool RemotePacketHandler::writePacket(BitStream& stream, PacketType packetType) {
		switch (packetType) {
			case PacketType::remoteBaro:
				return writeRemoteBaroPacket(stream);
				
				break;
			default: {
				ESP_LOGE(_logTag, "failed to write packet: unsupported type %d", std::to_underlying(packetType));
				
				return false;
			}
		}
	}
	
	bool RemotePacketHandler::writeRemoteBaroPacket(BitStream& stream) {
		auto& rc = RC::getInstance();
		auto& settings = rc.getSettings();
		
		// 1 hectopascal ~= 7.88 meters ~= 30 feet of altitude, which is not good enough for low altitude RC aircraft
		// So we'll be using decapascals. Cool fact: The maximum recorded atmospheric pressure on Earth, adjusted to sea level,
		// is around 1085.7 hPa (32.09 inHg), occurring in Tonsontsengel, Mongolia (2001)
		// So in theory 1100 daPa will be more than enough
		stream.writeUint16(settings.controls.referencePressureSTD ? 10132 : settings.controls.referencePressurePa / 10);
		
		return true;
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
	
	void RemotePacketHandler::onConnectionStateChanged(TransceiverConnectionState fromState, TransceiverConnectionState toState) {
		auto& rc = RC::getInstance();
		
		switch (toState) {
			case TransceiverConnectionState::connected: {
				rc.getSpeaker().play(resources::Sounds::transceiverConnect());
				break;
			}
			case TransceiverConnectionState::disconnected: {
				rc.getSpeaker().play(resources::Sounds::transceiverDisconnect());
				break;
			}
			default: break;
		}
	}
}
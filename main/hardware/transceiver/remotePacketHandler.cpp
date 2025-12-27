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
			case PacketType::aircraftStatistics: {
				return readAircraftStatisticsPacket(stream, payloadLength);
			}
			case PacketType::aircraftAutopilot: {
				return readAircraftAutopilotPacket(stream, payloadLength);
			}
			default: {
				ESP_LOGE(_logTag, "failed to read packet: unsupported type %d", std::to_underlying(packetType));
				
				return false;
			}
		}
	}
	
	bool RemotePacketHandler::readAircraftADIRSPacket(BitStream& stream, uint8_t payloadLength) {
		if (!validatePayloadChecksumAndLength(
			stream,
			AircraftADIRSPacket::rollLengthBits
				+ AircraftADIRSPacket::pitchLengthBits
				+ AircraftADIRSPacket::yawLengthBits
				+ AircraftADIRSPacket::slipAndSkidLengthBits
				+ AircraftADIRSPacket::speedLengthBits
				+ AircraftADIRSPacket::altitudeLengthBits,
			payloadLength
		))
			return false;
		
		auto& rc = RC::getInstance();
		auto& ad = rc.getAircraftData();
		
		const auto time = esp_timer_get_time();
		const auto deltaTime = time - _aircraftADIRSPacketTime;
		_aircraftADIRSPacketTime = time;
		
		const auto oldSpeed = ad.raw.airSpeedMps;
		const auto oldAltitude = ad.raw.geographicCoordinates.getAltitude();
		
		// Roll
		ad.raw.rollRad = readRadians(stream, AircraftADIRSPacket::rollLengthBits);
		
		// Pitch
		ad.raw.pitchRad = readRadians(stream, AircraftADIRSPacket::pitchLengthBits);
		
		// Yaw
		ad.raw.yawRad = readRadians(stream, AircraftADIRSPacket::yawLengthBits);
		
		// Slip & skid
		auto slipAndSkidFactor =
			static_cast<float>(stream.readUint8(AircraftADIRSPacket::slipAndSkidLengthBits))
			/ static_cast<float>(1 << AircraftADIRSPacket::slipAndSkidLengthBits);
		
		ad.raw.slipAndSkidG =
			-static_cast<float>(AircraftADIRSPacket::slipAndSkidMaxG)
			+ static_cast<float>(AircraftADIRSPacket::slipAndSkidMaxG) * 2
			* slipAndSkidFactor;
		
		// Speed
		ad.raw.airSpeedMps = static_cast<float>(stream.readUint8(AircraftADIRSPacket::speedLengthBits));
		
		// Altitude
		const auto altitudeFactor =
			static_cast<float>(stream.readUint16(AircraftADIRSPacket::altitudeLengthBits))
			/ static_cast<float>(1 << AircraftADIRSPacket::altitudeLengthBits);
		
		ad.raw.geographicCoordinates.setAltitude(AircraftADIRSPacket::altitudeMin + (AircraftADIRSPacket::altitudeMax - AircraftADIRSPacket::altitudeMin) * altitudeFactor);

		// Value conversions
//		ad.windSpeed = Units::convertSpeed(packet->windSpeedMs, SpeedUnit::meterPerSecond, SpeedUnit::knot);
		
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
		const auto deltaAltitude = ad.raw.geographicCoordinates.getAltitude() - oldAltitude;
		
		// Airspeed & altitude, 5 sec
		ad.raw.airSpeedTrendMPS = (ad.raw.airSpeedMps - oldSpeed) * 5'000'000.f / static_cast<float>(deltaTime);
		ad.raw.altitudeTrendM = deltaAltitude * 5'000'000.f / static_cast<float>(deltaTime);
		
		// Vertical speed, 1 min
		ad.raw.verticalSpeedMPM = deltaAltitude * 60'000'000.f / static_cast<float>(deltaTime);
		
		return true;
	}
	
	bool RemotePacketHandler::readAircraftStatisticsPacket(BitStream& stream, uint8_t payloadLength) {
		if (!validatePayloadChecksumAndLength(
			stream,
			AircraftStatisticsPacket::throttleLengthBits
				+ AircraftStatisticsPacket::latLengthBits
				+ AircraftStatisticsPacket::lonLengthBits
				+ AircraftStatisticsPacket::batteryLengthBits,
			payloadLength
		))
			return false;
		
		auto& rc = RC::getInstance();
		auto& ad = rc.getAircraftData();
		
		// Throttle
		ad.raw.throttlePercent_0_255 = stream.readUint8(AircraftStatisticsPacket::throttleLengthBits) * 100 / (1 << AircraftStatisticsPacket::throttleLengthBits);

		// Latitude
		
		// 25 bits per [-90; 90] deg
		// [0.0; 1.0]
		const auto latFactor = static_cast<float>(stream.readUint32(AircraftStatisticsPacket::latLengthBits)) / static_cast<float>(1 << AircraftStatisticsPacket::latLengthBits);
		// [-pi / 2; pi / 2]
		const auto lat = latFactor * std::numbers::pi_v<float> - std::numbers::pi_v<float> / 2.f;
		ad.raw.geographicCoordinates.setLatitude(lat);
		
		// Longitude
		
		// 26 bits per [0; 360] deg
		// [0.0; 1.0]
		const auto lonFactor = static_cast<float>(stream.readUint32(AircraftStatisticsPacket::lonLengthBits)) / static_cast<float>(1 << AircraftStatisticsPacket::lonLengthBits);
		// [-pi; pi]
		const auto lon = lonFactor * std::numbers::pi_v<float> * 2;
		ad.raw.geographicCoordinates.setLongitude(lon);
		
		// Battery
		ad.raw.batteryVoltageV = static_cast<float>(stream.readUint16(AircraftStatisticsPacket::batteryLengthBits)) / 10.f;
		
		return true;
	}
	
	bool RemotePacketHandler::readAircraftAutopilotPacket(BitStream& stream, uint8_t payloadLength) {
		if (!validatePayloadChecksumAndLength(stream, 7 + 10 + 10, payloadLength))
			return false;
		
		auto& rc = RC::getInstance();
		auto& ad = rc.getAircraftData();
		
		
		ad.raw.flightDirectorRollRad = readRadians(stream, 10);
		ad.raw.flightDirectorPitchRad = readRadians(stream, 10);
		
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
	
	float RemotePacketHandler::readRadians(BitStream& stream, uint8_t bits) {
		auto value = static_cast<float>(stream.readUint16(bits)) / static_cast<float>(1 << bits);
		value = value - 0.5f;
		value = value * (2 * std::numbers::pi_v<float>);
		
		return sanitizeValue<float>(value, toRadians(-180), toRadians(180));
	}
}
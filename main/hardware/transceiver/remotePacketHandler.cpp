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
		
		const auto oldSpeed = ad.raw.airSpeedMPS;
		const auto oldAltitude = ad.raw.geographicCoordinates.getAltitude();
		
		const auto readRad = [&stream](uint8_t bits) {
			auto value = static_cast<float>(stream.readUint16(bits)) / static_cast<float>((1 << bits) - 1);
			value = value - 0.5f;
			value = value * (2 * std::numbers::pi_v<float>);
			
			return sanitizeValue<float>(value, toRadians(-180), toRadians(180));
		};
		
		// Roll / pitch / yaw
		ad.raw.rollRad = readRad(AircraftADIRSPacket::rollLengthBits);
		ad.raw.pitchRad = readRad(AircraftADIRSPacket::pitchLengthBits);
		ad.raw.yawRad = readRad(AircraftADIRSPacket::yawLengthBits);
		
		// Slip & skid
		ad.raw.slipAndSkidFactor =
			// [0.0, 1.0]
			static_cast<float>(stream.readUint8(AircraftADIRSPacket::slipAndSkidLengthBits))
			/ static_cast<float>((1 << AircraftADIRSPacket::slipAndSkidLengthBits) - 1)
			// [-1.0; 1.0]
			* 2.f - 1.f;
		
		// Speed
		ad.raw.airSpeedMPS = static_cast<float>(stream.readUint8(AircraftADIRSPacket::speedLengthBits));
		
		// Altitude
		const auto altitudeFactor =
			static_cast<float>(stream.readUint16(AircraftADIRSPacket::altitudeLengthBits))
			/ static_cast<float>((1 << AircraftADIRSPacket::altitudeLengthBits) - 1);
		
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
//		ad.windDirection = toRadians(packet->windDirectionDeg);
		
		// Trends
		const auto deltaAltitudeM = ad.raw.geographicCoordinates.getAltitude() - oldAltitude;
		
		// Airspeed & altitude, 5 sec
		ad.raw.airSpeedTrendMPS = (ad.raw.airSpeedMPS - oldSpeed) * 5'000'000.f / static_cast<float>(deltaTime);
		ad.raw.altitudeTrendM = deltaAltitudeM * 5'000'000.f / static_cast<float>(deltaTime);
		
		// Vertical speed, 1 min
		ad.raw.verticalSpeedMPM = deltaAltitudeM * 60'000'000.f / static_cast<float>(deltaTime);
		
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
		
		// -------------------------------- Throttle --------------------------------
		
		ad.raw.throttlePercent_0_255 = stream.readUint8(AircraftStatisticsPacket::throttleLengthBits) * 0xFF / ((1 << AircraftStatisticsPacket::throttleLengthBits) - 1);
		
		// -------------------------------- Latitude --------------------------------
		
		// 25 bits per [-90; 90] deg
		// [0.0; 1.0]
		const auto latFactor =
			static_cast<float>(stream.readUint32(AircraftStatisticsPacket::latLengthBits))
			/ static_cast<float>((1 << AircraftStatisticsPacket::latLengthBits) - 1);
		
		// [-pi / 2; pi / 2]
		const auto lat = latFactor * std::numbers::pi_v<float> - std::numbers::pi_v<float> / 2.f;
		ad.raw.geographicCoordinates.setLatitude(lat);
		
		// -------------------------------- Longitude --------------------------------
		
		// 26 bits per [0; 360] deg
		// [0.0; 1.0]
		const auto lonFactor =
			static_cast<float>(stream.readUint32(AircraftStatisticsPacket::lonLengthBits))
			/ static_cast<float>((1 << AircraftStatisticsPacket::lonLengthBits) - 1);
		
		// [-pi; pi]
		const auto lon = lonFactor * std::numbers::pi_v<float> * 2;
		ad.raw.geographicCoordinates.setLongitude(lon);
		
		// -------------------------------- Battery --------------------------------
		
		const auto batteryVoltageDaV = stream.readUint16(AircraftStatisticsPacket::batteryLengthBits);
		ad.raw.batteryVoltageV = static_cast<float>(batteryVoltageDaV) / 10.f;
		
		return true;
	}
	
	bool RemotePacketHandler::readAircraftAutopilotPacket(BitStream& stream, uint8_t payloadLength) {
		if (!validatePayloadChecksumAndLength(stream, 7 + 10 + 10, payloadLength))
			return false;
		
		auto& rc = RC::getInstance();
		auto& ad = rc.getAircraftData();
		
		return true;
	}
	
	// -------------------------------- Writing --------------------------------
	
	bool RemotePacketHandler::writePacket(BitStream& stream, PacketType packetType) {
		switch (packetType) {
			case PacketType::remoteChannelsData:
				return writeRemoteChannelsDataPacket(stream);
				
			case PacketType::remoteAuxiliary:
				return writeRemoteAuxiliary0Packet(stream);
				
			default: {
				ESP_LOGE(_logTag, "failed to writeNext packet: unsupported type %d", std::to_underlying(packetType));
				
				return false;
			}
		}
	}
	
	bool RemotePacketHandler::writeRemoteChannelsDataPacket(BitStream& stream) {
		auto& rc = RC::getInstance();
		auto& rd = rc.getRemoteData();
		
		// Motors
		
		// Throttle
		stream.writeUint16(
			rd.raw.throttle_0_255 * ((1 << RemoteChannelsPacket::motorLengthBits) - 1) / 0xFF,
			RemoteChannelsPacket::motorLengthBits
		);
		
		const auto writeAxis = [&stream](uint16_t axisValue) {
			stream.writeUint16(
				static_cast<uint16_t>(axisValue * ((1 << RemoteChannelsPacket::motorLengthBits) - 1) / Axis::maxValue),
				RemoteChannelsPacket::motorLengthBits
			);
		};
		
		writeAxis(rc.getJoystickHorizontal().getProcessedValue());
		writeAxis(rc.getJoystickVertical().getProcessedValue());
		writeAxis(rc.getRing().getProcessedValue());
		writeAxis(rc.getLeverLeft().getProcessedValue());
		writeAxis(rc.getLeverRight().getProcessedValue());
		
		// Lights
		stream.writeBool(rd.raw.navigationLights);
		stream.writeBool(rd.raw.strobeLights);
		stream.writeBool(rd.raw.landingLights);
		stream.writeBool(rd.raw.cabinLights);
		
		return true;
	}
	
	bool RemotePacketHandler::writeRemoteAuxiliary0Packet(BitStream& stream) {
		auto& rc = RC::getInstance();
		auto& settings = rc.getSettings();
		
		// 1 hectopascal ~= 7.88 meters ~= 30 feet of altitude, which is not good enough for low altitude RC aircraft
		// So we'll be using decapascals. Cool fact: The maximum recorded atmospheric pressure on Earth, adjusted to sea level,
		// is around 1085.7 hPa (32.09 inHg), occurring in Tonsontsengel, Mongolia (2001)
		// So in theory 1100 daPa will be more than enough
		// So 14 bits
		stream.writeUint16(settings.controls.referencePressureSTD ? 10132 : settings.controls.referencePressurePa / 10, RemoteAuxiliary0Packet::referencePressureLengthBits);
		
		return true;
	}
	
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
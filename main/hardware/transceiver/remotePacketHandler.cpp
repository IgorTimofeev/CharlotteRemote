#include "remotePacketHandler.h"

#include <utility>

#include <esp_timer.h>

#include "rc.h"
#include "resources/sounds.h"

namespace pizda {
	// -------------------------------- PacketSequenceItem --------------------------------
	
	PacketSequenceItem::PacketSequenceItem(RemotePacketType type, uint8_t count, bool useEnqueued) : _type(type), _count(count), _useEnqueued(useEnqueued) {
	
	}
	
	RemotePacketType PacketSequenceItem::getType() const {
		return _type;
	}
	
	uint8_t PacketSequenceItem::getCount() const {
		return _count;
	}
	
	bool PacketSequenceItem::useEnqueued() const {
		return _useEnqueued;
	}
	
	// -------------------------------- Generic --------------------------------
	
	void RemotePacketHandler::onStart() {
		ESP_LOGI(_logTag, "started");
		
		while (true) {
			transmit(100'000);
			receive(100'000);
		}
	}
	
	void RemotePacketHandler::onIsConnectedChanged() {
		auto& rc = RC::getInstance();
		
		if (isConnected()) {
			rc.getSpeaker().play(resources::Sounds::transceiverConnect());
		}
		else {
			rc.getSpeaker().play(resources::Sounds::transceiverDisconnect());
		}
	}
	
	// -------------------------------- Receiving --------------------------------
	
	bool RemotePacketHandler::onReceive(BitStream& stream, uint8_t packetType, uint8_t payloadLength) {
		switch (static_cast<AircraftPacketType>(packetType)) {
			case AircraftPacketType::aircraftADIRS:
				return receiveAircraftADIRSPacket(stream, payloadLength);
				
			case AircraftPacketType::aircraftAuxiliary:
				return receiveAircraftAuxiliaryPacket(stream, payloadLength);
				
			case AircraftPacketType::aircraftAutopilot:
				return receiveAircraftAutopilotPacket(stream, payloadLength);
				
			case AircraftPacketType::aircraftCalibration:
				return receiveAircraftCalibrationPacket(stream, payloadLength);
				
			default:
				ESP_LOGE(_logTag, "failed to receive packet: unsupported type %d", packetType);
				
				return false;
		}
	}
	
	bool RemotePacketHandler::receiveAircraftADIRSPacket(BitStream& stream, uint8_t payloadLength) {
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
		
		const auto time = esp_timer_get_time();
		const auto deltaTime = time - _aircraftADIRSPacketTime;
		_aircraftADIRSPacketTime = time;
		
		auto& rc = RC::getInstance();
		auto& ad = rc.getAircraftData();
		
		const auto oldSpeed = ad.raw.airSpeedMPS;
		const auto oldAltitude = ad.raw.geographicCoordinates.getAltitude();
		
		// Roll / pitch / yaw
		ad.raw.rollRad = readRadians(stream, AircraftADIRSPacket::rollRangeRad, AircraftADIRSPacket::rollLengthBits);
		ad.raw.pitchRad = readRadians(stream, AircraftADIRSPacket::pitchRangeRad, AircraftADIRSPacket::pitchLengthBits);
		ad.raw.yawRad = readRadians(stream, AircraftADIRSPacket::yawRangeRad, AircraftADIRSPacket::yawLengthBits);
		
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
	
	bool RemotePacketHandler::receiveAircraftAuxiliaryPacket(BitStream& stream, uint8_t payloadLength) {
		if (!validatePayloadChecksumAndLength(
			stream,
			AircraftAuxiliaryPacket::throttleLengthBits
			+ AircraftAuxiliaryPacket::latLengthBits
			+ AircraftAuxiliaryPacket::lonLengthBits
			+ AircraftAuxiliaryPacket::batteryLengthBits,
			payloadLength
		))
			return false;
		
		auto& rc = RC::getInstance();
		auto& ad = rc.getAircraftData();
		
		// -------------------------------- Throttle --------------------------------
		
		ad.raw.throttlePercent_0_255 = stream.readUint8(AircraftAuxiliaryPacket::throttleLengthBits) * 0xFF / ((1 << AircraftAuxiliaryPacket::throttleLengthBits) - 1);
		
		// -------------------------------- Latitude --------------------------------
		
		// 25 bits per [-90; 90] deg
		// [0.0; 1.0]
		const auto latFactor =
			static_cast<float>(stream.readUint32(AircraftAuxiliaryPacket::latLengthBits))
			/ static_cast<float>((1 << AircraftAuxiliaryPacket::latLengthBits) - 1);
		
		// [-pi / 2; pi / 2]
		const auto lat = latFactor * std::numbers::pi_v<float> - std::numbers::pi_v<float> / 2.f;
		ad.raw.geographicCoordinates.setLatitude(lat);
		
		// -------------------------------- Longitude --------------------------------
		
		// 26 bits per [0; 360] deg
		// [0.0; 1.0]
		const auto lonFactor =
			static_cast<float>(stream.readUint32(AircraftAuxiliaryPacket::lonLengthBits))
			/ static_cast<float>((1 << AircraftAuxiliaryPacket::lonLengthBits) - 1);
		
		// [-pi; pi]
		const auto lon = lonFactor * std::numbers::pi_v<float> * 2;
		ad.raw.geographicCoordinates.setLongitude(lon);
		
		// -------------------------------- Battery --------------------------------
		
		const auto batteryVoltageDaV = stream.readUint16(AircraftAuxiliaryPacket::batteryLengthBits);
		ad.raw.batteryVoltageV = static_cast<float>(batteryVoltageDaV) / 10.f;
		
		return true;
	}
	
	bool RemotePacketHandler::receiveAircraftAutopilotPacket(BitStream& stream, uint8_t payloadLength) {
		if (!validatePayloadChecksumAndLength(
			stream,
			AircraftAutopilotPacket::flightDirectorRollLengthBits
				+ AircraftAutopilotPacket::flightDirectorPitchLengthBits,
			payloadLength
		))
			return false;
		
		auto& rc = RC::getInstance();
		auto& ad = rc.getAircraftData();
		
		ad.raw.flightDirectorRollRad = readRadians(stream, AircraftAutopilotPacket::flightDirectorRollRangeRad, AircraftAutopilotPacket::flightDirectorRollLengthBits);
		ad.raw.flightDirectorPitchRad = readRadians(stream, AircraftAutopilotPacket::flightDirectorPitchRangeRad, AircraftAutopilotPacket::flightDirectorPitchLengthBits);
		
		return true;
	}
	
	bool RemotePacketHandler::receiveAircraftCalibrationPacket(BitStream& stream, uint8_t payloadLength) {
		if (!validatePayloadChecksumAndLength(stream, 1, payloadLength))
			return false;
		
		auto& rc = RC::getInstance();
		auto& ad = rc.getAircraftData();
		
		return true;
	}
	
	// -------------------------------- Transmitting --------------------------------
	
	uint8_t RemotePacketHandler::getTransmitPacketType() {
		switch (getAircraftState()) {
			case AircraftState::aircraftCalibrating: {
				return std::to_underlying(RemotePacketType::NOP);
			}
			default: {
				const auto& item = _packetSequence[_packetSequenceIndex];
				
				const auto next = [this, &item]() {
					_packetSequenceItemCounter++;
					
					if (_packetSequenceItemCounter < item.getCount())
						return;
					
					_packetSequenceItemCounter = 0;
					
					_packetSequenceIndex++;
					
					if (_packetSequenceIndex >= _packetSequence.size())
						_packetSequenceIndex = 0;
				};
				
				// Enqueued
				if (item.useEnqueued() && !_packetQueue.empty()) {
					const auto packetType = _packetQueue.front();
					_packetQueue.pop();
					
					next();
					
					return std::to_underlying(packetType);
				}
				// Normal
				else {
					const auto packetType = item.getType();
					
					next();
					
					return std::to_underlying(packetType);
				}
			}
		}
	}
	
	bool RemotePacketHandler::onTransmit(BitStream& stream, uint8_t packetType) {
		switch (static_cast<RemotePacketType>(packetType)) {
			case RemotePacketType::NOP:
				return transmitNOPPacket(stream);
			
			case RemotePacketType::remoteChannelsData:
				return transmitRemoteChannelsDataPacket(stream);
				
			case RemotePacketType::remoteAuxiliary:
				return transmitRemoteAuxiliaryPacket(stream);
			
			case RemotePacketType::remoteAutopilot:
				return transmitRemoteAutopilotPacket(stream);
				
			default:
				ESP_LOGE(_logTag, "failed to writeNext packet: unsupported type %d", packetType);
				
				return false;
		}
	}
	
	bool RemotePacketHandler::transmitNOPPacket(BitStream& stream) {
		return true;
	}
	
	bool RemotePacketHandler::transmitRemoteChannelsDataPacket(BitStream& stream) {
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
	
	bool RemotePacketHandler::transmitRemoteAuxiliaryPacket(BitStream& stream) {
		auto& rc = RC::getInstance();
		auto& settings = rc.getSettings();
		
		// 1 hectopascal ~= 7.88 meters ~= 30 feet of altitude, which is not good enough for low altitude RC aircraft
		// So we'll be using decapascals. Cool fact: The maximum recorded atmospheric pressure on Earth, adjusted to sea level,
		// is around 1085.7 hPa (32.09 inHg), occurring in Tonsontsengel, Mongolia (2001)
		// So in theory 1100 daPa will be more than enough
		// So 14 bits
		stream.writeUint16(settings.controls.referencePressureSTD ? 10132 : settings.controls.referencePressurePa / 10, RemoteAuxiliaryPacket::referencePressureLengthBits);
		
		return true;
	}
	
	bool RemotePacketHandler::transmitRemoteAutopilotPacket(BitStream& stream) {
		auto& rc = RC::getInstance();
		auto& settings = rc.getSettings();
		
		// Speed
		const auto speedMPS = Units::convertSpeed(settings.autopilot.speedKt, SpeedUnit::knot, SpeedUnit::meterPerSecond);
		stream.writeUint8(static_cast<uint8_t>(speedMPS), RemoteAutopilotPacket::speedLengthBits);
		stream.writeBool(settings.autopilot.autoThrottle);
		
		// Heading
		stream.writeUint16(static_cast<uint8_t>(settings.autopilot.headingDeg), RemoteAutopilotPacket::headingLengthBits);
		stream.writeBool(settings.autopilot.headingHold);
		
		// Altitude
		const auto altitudeM = Units::convertDistance(settings.autopilot.altitudeFt, DistanceUnit::foot, DistanceUnit::meter);
		const auto altitudeClamped = std::clamp<float>(altitudeM, RemoteAutopilotPacket::altitudeMin, RemoteAutopilotPacket::altitudeMax);
		
		const auto altitudeFactor =
			(altitudeClamped - static_cast<float>(RemoteAutopilotPacket::altitudeMin))
			/ static_cast<float>(RemoteAutopilotPacket::altitudeMax - AircraftADIRSPacket::altitudeMin);
		
		const auto altitudeValue = static_cast<uint16_t>(altitudeFactor * static_cast<float>((1 << RemoteAutopilotPacket::altitudeLengthBits) - 1));
		
		stream.writeUint16(altitudeValue, RemoteAutopilotPacket::altitudeLengthBits);
		stream.writeBool(settings.autopilot.levelChange);
		
		return true;
	}
	
	float RemotePacketHandler::readRadians(BitStream& stream, float range, uint8_t bits) {
		auto value = static_cast<float>(stream.readUint16(bits)) / static_cast<float>((1 << bits) - 1);
		value = value - 0.5f;
		value = value * range;
		
		return sanitizeValue<float>(value, toRadians(-180), toRadians(180));
	}
	
	void RemotePacketHandler::writeRadians(BitStream& stream, float value, float range, uint8_t bits) {
		const auto uintValue = static_cast<uint16_t>((value / range + 0.5f) * ((1 << bits) - 1));
		
		stream.writeUint16(uintValue, bits);
	}
}
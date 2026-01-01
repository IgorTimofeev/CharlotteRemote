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
			rc.getAudioPlayer().play(resources::sounds::transceiverConnect);
		}
		else {
			rc.getAudioPlayer().play(resources::sounds::transceiverDisconnect);
		}
	}
	
	// -------------------------------- Receiving --------------------------------
	
	bool RemotePacketHandler::onReceive(BitStream& stream, AircraftPacketType packetType, uint8_t payloadLength) {
		switch (packetType) {
			case AircraftPacketType::ADIRS:
				return receiveAircraftADIRSPacket(stream, payloadLength);
			
			case AircraftPacketType::auxiliary:
				return receiveAircraftAuxiliaryPacket(stream, payloadLength);
				
			case AircraftPacketType::calibration:
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
				+ AircraftADIRSPacket::altitudeLengthBits
				+ AircraftADIRSPacket::throttleLengthBits
				+ AircraftADIRSPacket::autopilotRollLengthBits
				+ AircraftADIRSPacket::autopilotPitchLengthBits,
			payloadLength
		))
			return false;
		
		const auto time = esp_timer_get_time();
		const auto deltaTime = time - _aircraftADIRSPacketTime;
		_aircraftADIRSPacketTime = time;
		
		auto& rc = RC::getInstance();
		auto& ad = rc.getAircraftData();
		
		const auto airspeedPrevMPS = ad.raw.airspeedMPS;
		const auto altitudePrevM = ad.raw.geographicCoordinates.getAltitude();
		
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
		const auto speedFactor =
			static_cast<float>(stream.readUint8(AircraftADIRSPacket::speedLengthBits))
			/ static_cast<float>((1 << AircraftADIRSPacket::speedLengthBits) - 1);
		
		ad.raw.airspeedMPS = static_cast<float>(AircraftADIRSPacket::speedMaxMPS) * speedFactor;
		
		// Altitude
		const auto altitudeFactor =
			static_cast<float>(stream.readUint16(AircraftADIRSPacket::altitudeLengthBits))
			/ static_cast<float>((1 << AircraftADIRSPacket::altitudeLengthBits) - 1);
		
		ad.raw.geographicCoordinates.setAltitude(AircraftADIRSPacket::altitudeMinM + (AircraftADIRSPacket::altitudeMaxM - AircraftADIRSPacket::altitudeMinM) * altitudeFactor);
		
		// Throttle
		ad.raw.throttle_0_255 = stream.readUint8(AircraftADIRSPacket::throttleLengthBits) * 0xFF / ((1 << AircraftADIRSPacket::throttleLengthBits) - 1);
		
		// Autopilot roll
		ad.raw.autopilotRollRad = readRadians(stream, AircraftADIRSPacket::autopilotRollRangeRad, AircraftADIRSPacket::autopilotRollLengthBits);
		
		// Autopilot pitch
		ad.raw.autopilotPitchRad = readRadians(stream, AircraftADIRSPacket::autopilotPitchRangeRad, AircraftADIRSPacket::autopilotPitchLengthBits);
		
		// Value conversions
//		ad.windSpeed = Units::convertSpeed(packet->windSpeedMs, SpeedUnit::meterPerSecond, SpeedUnit::knot);
		
//		ad.groundSpeedKt = Units::convertSpeed(packet->groundSpeedMs, SpeedUnit::meterPerSecond, SpeedUnit::knot);

//		ad.flightPathVectorPitch = packet->flightPathPitchRad;
//		ad.flightPathVectorYaw = packet->flightPathYawRad;
//
//		ad.flightDirectorPitch = packet->flightDirectorPitchRad;
//		ad.flightDirectorRoll = packet->flightDirectorRollRad;
//
//		ad.windDirection = toRadians(packet->windDirectionDeg);
		
		// Trends
		const auto deltaAltitudeM = ad.raw.geographicCoordinates.getAltitude() - altitudePrevM;
		
		// Airspeed & altitude, 5 sec
		ad.raw.airspeedTrendMPS = (ad.raw.airspeedMPS - airspeedPrevMPS) * 5'000'000.f / static_cast<float>(deltaTime);
		ad.raw.altitudeTrendM = deltaAltitudeM * 5'000'000.f / static_cast<float>(deltaTime);
		
		// Vertical speed, 1 min
		ad.raw.verticalSpeedMPM = deltaAltitudeM * 60'000'000.f / static_cast<float>(deltaTime);
		
		return true;
	}
	
	bool RemotePacketHandler::receiveAircraftAuxiliaryPacket(BitStream& stream, uint8_t payloadLength) {
		if (!validatePayloadChecksumAndLength(
			stream,
			+ AircraftAuxiliaryPacket::latLengthBits
				+ AircraftAuxiliaryPacket::lonLengthBits
				+ AircraftAuxiliaryPacket::batteryLengthBits,
			payloadLength
		))
			return false;
		
		auto& rc = RC::getInstance();
		auto& ad = rc.getAircraftData();

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
	
	bool RemotePacketHandler::receiveAircraftCalibrationPacket(BitStream& stream, uint8_t payloadLength) {
		if (!validatePayloadChecksumAndLength(stream, 1, payloadLength))
			return false;
		
		auto& rc = RC::getInstance();
		auto& ad = rc.getAircraftData();
		
		return true;
	}
	
	// -------------------------------- Transmitting --------------------------------
	
	RemotePacketType RemotePacketHandler::getTransmitPacketType() {
		switch (getRemoteState()) {
			case AircraftState::aircraftCalibrating: {
				return RemotePacketType::NOP;
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
					
					return packetType;
				}
				// Normal
				else {
					const auto packetType = item.getType();
					
					next();
					
					return packetType;
				}
			}
		}
	}
	
	bool RemotePacketHandler::onTransmit(BitStream& stream, RemotePacketType packetType) {
		switch (packetType) {
			case RemotePacketType::NOP:
				return transmitNOPPacket(stream);
			
			case RemotePacketType::channelsData:
				return transmitRemoteChannelsDataPacket(stream);
				
			case RemotePacketType::auxiliary:
				return transmitRemoteAuxiliaryPacket(stream);
				
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
		
		// Reference pressure
		stream.writeUint16(settings.controls.referencePressureSTD ? 10132 : settings.controls.referencePressurePa / 10, RemoteAuxiliaryPacket::referencePressureLengthBits);
		
		// -------------------------------- Autopilot --------------------------------
		
		// Speed
		const auto speedFactor =
			std::min<float>(
				Units::convertSpeed(settings.autopilot.speedKt, SpeedUnit::knot, SpeedUnit::meterPerSecond),
				RemoteAuxiliaryPacket::autopilotSpeedMaxMPS
			)
			/ static_cast<float>(RemoteAuxiliaryPacket::autopilotSpeedMaxMPS);
		
		const auto speedMapped = static_cast<float>((1 << RemoteAuxiliaryPacket::autopilotSpeedLengthBits) - 1) * speedFactor;
		
		stream.writeUint8(static_cast<uint8_t>(std::round(speedMapped)), RemoteAuxiliaryPacket::autopilotSpeedLengthBits);
		stream.writeBool(settings.autopilot.autoThrottle);
		
		// Heading
		stream.writeUint16(settings.autopilot.headingDeg, RemoteAuxiliaryPacket::autopilotHeadingLengthBits);
		stream.writeBool(settings.autopilot.headingHold);
		
		// Altitude
		const auto altitudeM = Units::convertDistance(settings.autopilot.altitudeFt, DistanceUnit::foot, DistanceUnit::meter);
		const auto altitudeClamped = std::clamp<float>(altitudeM, RemoteAuxiliaryPacket::autopilotAltitudeMinM, RemoteAuxiliaryPacket::autopilotAltitudeMaxM);
		
		const auto altitudeFactor =
			(altitudeClamped - static_cast<float>(RemoteAuxiliaryPacket::autopilotAltitudeMinM))
			/ static_cast<float>(RemoteAuxiliaryPacket::autopilotAltitudeMaxM - AircraftADIRSPacket::altitudeMinM);
		
		const auto altitudeValue = static_cast<uint16_t>(std::round(altitudeFactor * static_cast<float>((1 << RemoteAuxiliaryPacket::autopilotAltitudeLengthBits) - 1)));
		
		stream.writeUint16(altitudeValue, RemoteAuxiliaryPacket::autopilotAltitudeLengthBits);
		stream.writeBool(settings.autopilot.levelChange);
		
		// Engaged
		stream.writeBool(rc.getRemoteData().raw.autopilotEngaged);
		
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
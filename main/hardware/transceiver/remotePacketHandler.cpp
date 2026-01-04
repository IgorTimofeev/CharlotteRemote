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
	
	void RemotePacketHandler::enqueue(RemotePacketType type) {
		_packetQueue.push(type);
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
		const auto altitudePrevM = ad.raw.coordinates.getAltitude();
		
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
		
		ad.raw.coordinates.setAltitude(AircraftADIRSPacket::altitudeMinM + (AircraftADIRSPacket::altitudeMaxM - AircraftADIRSPacket::altitudeMinM) * altitudeFactor);
		
		// Autopilot roll
		ad.raw.autopilot.rollRad = readRadians(stream, AircraftADIRSPacket::autopilotRollRangeRad, AircraftADIRSPacket::autopilotRollLengthBits);
		
		// Autopilot pitch
		ad.raw.autopilot.pitchRad = readRadians(stream, AircraftADIRSPacket::autopilotPitchRangeRad, AircraftADIRSPacket::autopilotPitchLengthBits);
		
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
		const auto deltaAltitudeM = ad.raw.coordinates.getAltitude() - altitudePrevM;
		
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
			AircraftAuxiliaryPacket::throttleLengthBits
				+ AircraftAuxiliaryPacket::latLengthBits
				+ AircraftAuxiliaryPacket::lonLengthBits
				+ AircraftAuxiliaryPacket::batteryLengthBits
				// Lights
				+ 4
				+ AircraftAuxiliaryPacket::autopilotLateralModeLengthBits
				+ AircraftAuxiliaryPacket::autopilotVerticalModeLengthBits
				// A/T
				+ 1
				// A/P
				+ 1,
			payloadLength
		))
			return false;
		
		auto& rc = RC::getInstance();
		auto& ad = rc.getAircraftData();
		
		// -------------------------------- Throttle --------------------------------
		
		ad.raw.throttle_0_255 =
			stream.readUint8(AircraftAuxiliaryPacket::throttleLengthBits)
			* 0xFF
			/ ((1 << AircraftAuxiliaryPacket::throttleLengthBits) - 1);
		
		// -------------------------------- Latitude --------------------------------
		
		// 25 bits per [-90; 90] deg
		// [0.0; 1.0]
		const auto latFactor =
			static_cast<float>(stream.readUint32(AircraftAuxiliaryPacket::latLengthBits))
			/ static_cast<float>((1 << AircraftAuxiliaryPacket::latLengthBits) - 1);
		
		// [-pi / 2; pi / 2]
		const auto lat = latFactor * std::numbers::pi_v<float> - std::numbers::pi_v<float> / 2.f;
		ad.raw.coordinates.setLatitude(lat);
		
		// -------------------------------- Longitude --------------------------------
		
		// 26 bits per [0; 360] deg
		// [0.0; 1.0]
		const auto lonFactor =
			static_cast<float>(stream.readUint32(AircraftAuxiliaryPacket::lonLengthBits))
			/ static_cast<float>((1 << AircraftAuxiliaryPacket::lonLengthBits) - 1);
		
		// [-pi; pi]
		const auto lon = lonFactor * std::numbers::pi_v<float> * 2;
		ad.raw.coordinates.setLongitude(lon);
		
		// -------------------------------- Battery --------------------------------
		
		const auto batteryVoltageDaV = stream.readUint16(AircraftAuxiliaryPacket::batteryLengthBits);
		ad.raw.batteryVoltageV = static_cast<float>(batteryVoltageDaV) / 10.f;
		
		// -------------------------------- Lights --------------------------------
		
		ad.raw.lights.navigation = stream.readBool();
		ad.raw.lights.strobe = stream.readBool();
		ad.raw.lights.landing = stream.readBool();
		ad.raw.lights.cabin = stream.readBool();
		
		// -------------------------------- Autopilot --------------------------------
		
		// Modes
		ad.raw.autopilot.lateralMode = static_cast<AutopilotLateralMode>(stream.readUint8(AircraftAuxiliaryPacket::autopilotLateralModeLengthBits));
		ad.raw.autopilot.verticalMode = static_cast<AutopilotVerticalMode>(stream.readUint8(AircraftAuxiliaryPacket::autopilotVerticalModeLengthBits));
		
		// Autothrottle
		ad.raw.autopilot.autothrottle = stream.readBool();
		
		// Autopilot
		ad.raw.autopilot.autopilot = stream.readBool();
		
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
			
			case RemotePacketType::controls:
				return transmitRemoteControlsPacket(stream);
			
			case RemotePacketType::lights:
				return transmitRemoteLightsPacket(stream);
			
			case RemotePacketType::baro:
				return transmitRemoteBaroPacket(stream);
				
			case RemotePacketType::autopilot:
				return transmitRemoteAutopilotPacket(stream);
				
			default:
				ESP_LOGE(_logTag, "failed to writeNext packet: unsupported type %d", packetType);
				
				return false;
		}
	}
	
	bool RemotePacketHandler::transmitNOPPacket(BitStream& stream) {
		return true;
	}
	
	bool RemotePacketHandler::transmitRemoteControlsPacket(BitStream& stream) {
		auto& rc = RC::getInstance();
		auto& rd = rc.getRemoteData();
		
		// Motors
		
		// Throttle
		stream.writeUint16(
			rd.throttle_0_255 * ((1 << RemoteControlsPacket::motorLengthBits) - 1) / 0xFF,
			RemoteControlsPacket::motorLengthBits
		);
		
		const auto writeAxis = [&stream](uint16_t axisValue) {
			stream.writeUint16(
				static_cast<uint16_t>(axisValue * ((1 << RemoteControlsPacket::motorLengthBits) - 1) / Axis::maxValue),
				RemoteControlsPacket::motorLengthBits
			);
		};
		
		writeAxis(rc.getJoystickHorizontal().getValue());
		writeAxis(rc.getJoystickVertical().getValue());
		writeAxis(rc.getRing().getValue());
		writeAxis(rc.getLeverRight().getValue());
		writeAxis(rc.getLeverLeft().getValue());
		
		return true;
	}
	
	bool RemotePacketHandler::transmitRemoteLightsPacket(BitStream& stream) {
		auto& rc = RC::getInstance();
		auto& rd = rc.getRemoteData();
		
		stream.writeBool(rd.lights.navigation);
		stream.writeBool(rd.lights.strobe);
		stream.writeBool(rd.lights.landing);
		stream.writeBool(rd.lights.cabin);
		
		return true;
	}
	
	bool RemotePacketHandler::transmitRemoteBaroPacket(BitStream& stream) {
		auto& rc = RC::getInstance();
		auto& settings = rc.getSettings();
		auto& rd = rc.getRemoteData();
		
		// Reference pressure
		stream.writeUint16(
			settings.controls.referencePressureSTD ? 10132 : settings.controls.referencePressurePa / 10,
			RemoteBaroPacket::referencePressureLengthBits
		);
		
		return true;
	}
	
	bool RemotePacketHandler::transmitRemoteAutopilotPacket(BitStream& stream) {
		auto& rc = RC::getInstance();
		auto& settings = rc.getSettings();
		auto& rd = rc.getRemoteData();
		
		// Speed
		const auto speedFactor =
			std::min<float>(
				Units::convertSpeed(settings.autopilot.speedKt, SpeedUnit::knot, SpeedUnit::meterPerSecond),
				RemoteAutopilotPacket::speedMaxMPS
			)
			/ static_cast<float>(RemoteAutopilotPacket::speedMaxMPS);
		
		const auto speedMapped = static_cast<float>((1 << RemoteAutopilotPacket::speedLengthBits) - 1) * speedFactor;
		
		stream.writeUint8(static_cast<uint8_t>(std::round(speedMapped)), RemoteAutopilotPacket::speedLengthBits);
		
		// Heading
		stream.writeUint16(settings.autopilot.headingDeg, RemoteAutopilotPacket::headingLengthBits);
		
		// Altitude
		const auto altitudeM = Units::convertDistance(settings.autopilot.altitudeFt, DistanceUnit::foot, DistanceUnit::meter);
		const auto altitudeClamped = std::clamp<float>(altitudeM, RemoteAutopilotPacket::altitudeMinM, RemoteAutopilotPacket::altitudeMaxM);
		
		const auto altitudeFactor =
			(altitudeClamped - static_cast<float>(RemoteAutopilotPacket::altitudeMinM))
			/ static_cast<float>(RemoteAutopilotPacket::altitudeMaxM - RemoteAutopilotPacket::altitudeMinM);
		
		const auto altitudeValue = static_cast<uint16_t>(std::round(altitudeFactor * static_cast<float>((1 << RemoteAutopilotPacket::altitudeLengthBits) - 1)));
		
		stream.writeUint16(altitudeValue, RemoteAutopilotPacket::altitudeLengthBits);
		
		// Modes
		stream.writeUint8(std::to_underlying(rd.autopilot.lateralMode), RemoteAutopilotPacket::lateralModeLengthBits);
		stream.writeUint8(std::to_underlying(rd.autopilot.verticalMode), RemoteAutopilotPacket::verticalModeLengthBits);
		
		// Autothrottle
		stream.writeBool(rd.autopilot.autothrottle);
		
		// Autopilot
		stream.writeBool(rd.autopilot.autopilot);
		
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
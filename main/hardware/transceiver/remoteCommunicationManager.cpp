#include "remoteCommunicationManager.h"

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
	
	void RemoteCommunicationManager::onStart() {
		ESP_LOGI(_logTag, "started");
		
		while (true) {
			transmit(100'000);
			receive(100'000);
		}
	}
	
	void RemoteCommunicationManager::onConnectionStateChanged() {
		auto& rc = RC::getInstance();
		
		if (isConnected()) {
			rc.getAudioPlayer().play(resources::sounds::transceiverConnect);
		}
		else {
			RC::getInstance().getAircraftData().raw.calibration.checkValidTime();
			
			rc.getAudioPlayer().play(resources::sounds::transceiverDisconnect);
		}
	}
	
	
	void RemoteCommunicationManager::enqueue(RemotePacketType type) {
		_enqueuedPackets.insert(type);
	}
	
	// -------------------------------- Receiving --------------------------------
	
	bool RemoteCommunicationManager::onReceive(BitStream& stream, AircraftPacketType packetType, uint8_t payloadLength) {
		RC::getInstance().getAircraftData().raw.calibration.checkValidTime();
		
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
	
	bool RemoteCommunicationManager::receiveAircraftADIRSPacket(BitStream& stream, uint8_t payloadLength) {
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
		ad.raw.coordinates.setAltitude(readAltitude(
			stream,
			AircraftADIRSPacket::altitudeLengthBits,
			AircraftADIRSPacket::altitudeMinM,
			AircraftADIRSPacket::altitudeMaxM
		));
		
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
	
	bool RemoteCommunicationManager::receiveAircraftAuxiliaryPacket(BitStream& stream, uint8_t payloadLength) {
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
				+ AircraftAuxiliaryPacket::autopilotAltitudeLengthBits
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
		
		// -------------------------------- Latitude & longitude --------------------------------
		
		// Latitude
		// 25 bits per [-90; 90] deg
		// [0.0; 1.0]
		const auto latFactor =
			static_cast<float>(stream.readUint32(AircraftAuxiliaryPacket::latLengthBits))
			/ static_cast<float>((1 << AircraftAuxiliaryPacket::latLengthBits) - 1);
		
		// [-pi / 2; pi / 2]
		const auto lat = latFactor * std::numbers::pi_v<float> - std::numbers::pi_v<float> / 2.f;
		ad.raw.coordinates.setLatitude(lat);
		
		// Longitude
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
		
		// Altitude for ALT/VNAV modes
		ad.raw.autopilot.targetAltitudeM = readAltitude(
			stream,
			AircraftAuxiliaryPacket::autopilotAltitudeLengthBits,
			AircraftAuxiliaryPacket::autopilotAltitudeMinM,
			AircraftAuxiliaryPacket::autopilotAltitudeMaxM
		);
		
		// Autothrottle
		ad.raw.autopilot.autothrottle = stream.readBool();
		
		// Autopilot
		ad.raw.autopilot.autopilot = stream.readBool();
		
		return true;
	}
	
	bool RemoteCommunicationManager::receiveAircraftCalibrationPacket(BitStream& stream, uint8_t payloadLength) {
		if (!validatePayloadChecksumAndLength(
			stream,
			AircraftCalibrationPacket::systemLengthBits
				+ AircraftCalibrationPacket::progressLengthBits,
			payloadLength
		))
			return false;
		
		auto& rc = RC::getInstance();
		auto& ad = rc.getAircraftData();
		
		ad.raw.calibration.system = static_cast<AircraftCalibrationSystem>(stream.readUint8(AircraftCalibrationPacket::systemLengthBits));
		ad.raw.calibration.progress = stream.readUint8(AircraftCalibrationPacket::progressLengthBits) * 0xFF / ((1 << AircraftCalibrationPacket::progressLengthBits) - 1);
		ad.raw.calibration.setCalibrating(ad.raw.calibration.progress < 0xFF);
		
//		ESP_LOGI(_logTag, "calibration progress: %d", ad.raw.calibration.progress * 100 / 0xFF);
		
		return true;
	}
	
	// -------------------------------- Transmitting --------------------------------
	
	RemotePacketType RemoteCommunicationManager::getTransmitPacketType() {
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
		if (item.useEnqueued() && !_enqueuedPackets.empty()) {
			const auto packetType = *_enqueuedPackets.begin();
			_enqueuedPackets.erase(packetType);
			
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
	
	bool RemoteCommunicationManager::onTransmit(BitStream& stream, RemotePacketType packetType) {
		switch (packetType) {
			case RemotePacketType::controls:
				transmitRemoteControlsPacket(stream);
				break;
			
			case RemotePacketType::trim:
				transmitRemoteTrimPacket(stream);
				break;
			
			case RemotePacketType::lights:
				transmitRemoteLightsPacket(stream);
				break;
			
			case RemotePacketType::baro:
				transmitRemoteBaroPacket(stream);
				break;
				
			case RemotePacketType::autopilot:
				transmitRemoteAutopilotPacket(stream);
				break;
			
			case RemotePacketType::motorConfiguration:
				transmitRemoteMotorConfigurationPacket(stream);
				break;
				
			case RemotePacketType::calibrate:
				transmitRemoteCalibratePacket(stream);
				break;
				
			default:
				ESP_LOGE(_logTag, "failed to transmit packet: unsupported type %d", packetType);
				
				return false;
		}
		
		return true;
	}
	
	void RemoteCommunicationManager::transmitRemoteControlsPacket(BitStream& stream) {
		auto& rc = RC::getInstance();
		
		// Motors
		
		// Throttle
		stream.writeUint16(
			rc.getRemoteData().throttle_0_255 * ((1 << RemoteControlsPacket::motorLengthBits) - 1) / 0xFF,
			RemoteControlsPacket::motorLengthBits
		);
		
		const auto writeAxis = [&stream](uint16_t axisValue) {
			stream.writeUint16(
				static_cast<uint16_t>(axisValue * ((1 << RemoteControlsPacket::motorLengthBits) - 1) / Axis::valueMax),
				RemoteControlsPacket::motorLengthBits
			);
		};
		
		writeAxis(rc.getJoystickHorizontal().getFilteredValue());
		writeAxis(rc.getJoystickVertical().getFilteredValue());
		writeAxis(rc.getRing().getFilteredValue());
		writeAxis(rc.getLeverRight().getFilteredValue());
		writeAxis(rc.getLeverLeft().getFilteredValue());
	}
	
	void RemoteCommunicationManager::transmitRemoteTrimPacket(BitStream& stream) {
		auto& rc = RC::getInstance();
		
		const auto write = [&stream](int8_t settingsValue) {
			stream.writeUint16(
				// Mapping from [-100; 100] to [0; bits]
				static_cast<uint16_t>(
					(static_cast<int32_t>(settingsValue) + 100)
					* ((1 << RemoteTrimPacket::valueLengthBits) - 1)
					/ 200
				),
				RemoteTrimPacket::valueLengthBits
			);
		};
		
		write(rc.getSettings().controls.aileronsTrim);
		write(rc.getSettings().controls.elevatorTrim);
		write(rc.getSettings().controls.rudderTrim);
	}
	
	void RemoteCommunicationManager::transmitRemoteLightsPacket(BitStream& stream) {
		auto& rc = RC::getInstance();
		
		stream.writeBool(rc.getRemoteData().lights.navigation);
		stream.writeBool(rc.getRemoteData().lights.strobe);
		stream.writeBool(rc.getRemoteData().lights.landing);
		stream.writeBool(rc.getRemoteData().lights.cabin);
	}
	
	void RemoteCommunicationManager::transmitRemoteBaroPacket(BitStream& stream) {
		auto& rc = RC::getInstance();
		auto& settings = rc.getSettings();
		
		// Reference pressure
		stream.writeUint16(
			settings.controls.referencePressureSTD ? 10132 : settings.controls.referencePressurePa / 10,
			RemoteBaroPacket::referencePressureLengthBits
		);
	}
	
	void RemoteCommunicationManager::transmitRemoteAutopilotPacket(BitStream& stream) {
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
		const auto altitudeFt =
			rd.autopilot.verticalMode == AutopilotVerticalMode::alt
			? rd.autopilot.altitudeHoldFt
			: settings.autopilot.altitudeFt;
		
		writeAltitude(
			stream,
			Units::convertDistance(altitudeFt, DistanceUnit::foot, DistanceUnit::meter),
			RemoteAutopilotPacket::altitudeLengthBits,
			RemoteAutopilotPacket::altitudeMinM,
			RemoteAutopilotPacket::altitudeMaxM
		);
		
		// Modes
		stream.writeUint8(std::to_underlying(rd.autopilot.lateralMode), RemoteAutopilotPacket::lateralModeLengthBits);
		stream.writeUint8(std::to_underlying(rd.autopilot.verticalMode), RemoteAutopilotPacket::verticalModeLengthBits);
		
		// Autothrottle
		stream.writeBool(rd.autopilot.autothrottle);
		
		// Autopilot
		stream.writeBool(rd.autopilot.autopilot);
	}
	
	void RemoteCommunicationManager::transmitRemoteMotorConfigurationPacket(BitStream& stream) {
		const auto& motors = RC::getInstance().getSettings().motors;
	
		const auto write = [&stream](const MotorConfiguration& motor) {
			stream.writeUint16(motor.min, RemoteMotorConfigurationPacket::minLengthBits);
			stream.writeUint16(motor.max, RemoteMotorConfigurationPacket::maxLengthBits);
			stream.writeBool(motor.reverse);
		};
		
		write(motors.throttle);
		write(motors.noseWheel);
		
		write(motors.flapLeft);
		write(motors.aileronLeft);
		
		write(motors.flapRight);
		write(motors.aileronRight);
		
		write(motors.tailLeft);
		write(motors.tailRight);
	}
	
	void RemoteCommunicationManager::transmitRemoteCalibratePacket(BitStream& stream) {
		auto& rc = RC::getInstance();
		
		stream.writeUint8(std::to_underlying(rc.getRemoteData().calibrationSystem), RemoteCalibratePacket::systemLengthBits);
		
//		ESP_LOGI(_logTag, "Sending calibrate packet");
	}
}
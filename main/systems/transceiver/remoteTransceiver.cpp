#include "systems/transceiver/remoteTransceiver.h"

#include <utility>
#include <algorithm>
#include <limits>

#include <esp_timer.h>

#include "rc.h"
#include "resources/sounds.h"

namespace pizda {
	// -------------------------------- PacketSequenceItem --------------------------------
	
	PacketSequenceItem::PacketSequenceItem(const RemotePacketType type, const uint8_t count, const bool useEnqueued) : _type(type), _count(count), _useEnqueued(useEnqueued) {
	
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

	bool RemoteTransceiver::stopSpectrumScanning() {
		RC::getInstance().getRemoteData().transceiver.spectrumScanning.state = RemoteDataRadioSpectrumScanningState::stopped;

		// Switching to standby
		auto error = _SX.setStandby();

		if (error != SX1262Error::none) {
			logSXError("setNormalMode() error", error);
			return false;
		}

		// Setting normal RX/TX frequency
		error = _SX.setRFFrequency(config::transceiver::RFFrequencyHz);

		if (error != SX1262Error::none) {
			logSXError("setNormalMode() error", error);
			return false;
		}

		return true;
	}

	void RemoteTransceiver::onSpectrumScanning() {
		auto& rc = RC::getInstance();
		auto& st = rc.getSettings().transceiver.spectrumScanning;
		auto& rd = rc.getRemoteData().transceiver.spectrumScanning;

		// None -> requested
		if (rd.state == RemoteDataRadioSpectrumScanningState::startRequested) {
			ESP_LOGI(_logTag, "onSpectrumScanning() started, from: %lu, to: %lu, step: %lu", st.frequency.from, st.frequency.to, st.frequency.step);

			rd.state = RemoteDataRadioSpectrumScanningState::started;
			rd.historyMin = 0;
			rd.historyMax = std::numeric_limits<int8_t>::min();

			_spectrumScanningHistoryIndex = 0;
			_spectrumScanningSampleRSSISum = 0;
			_spectrumScanningSampleQuantity = 0;

			// Switching to standby
			const auto error = _SX.setStandby();

			if (error != SX1262Error::none) {
				logSXError("setSpectrumScanningMode() error", error);
				stopSpectrumScanning();

				return;
			}
		}
		else if (rd.state == RemoteDataRadioSpectrumScanningState::stopRequested) {
			stopSpectrumScanning();

			return;
		}

		// ESP_LOGI(_logTag, "onSpectrumScanning() moving to freq: %lu", ss.frequency.value);

		// Changing frequency
		auto error = _SX.setRFFrequency(rd.frequency);
		if (error != SX1262Error::none) {
			logSXError("getSpectrumScanningRSSI() error", error);
			stopSpectrumScanning();

			return;
		}

		vTaskDelay(pdMS_TO_TICKS(10));

		// Moving to RX single mode
		error = _SX.setRX();

		if (error != SX1262Error::none) {
			logSXError("getSpectrumScanningRSSI() error", error);
			stopSpectrumScanning();

			return;
		}

		// Applying RSSI inst multisampling
		float RSSIF;
		constexpr static uint8_t samplesLength = 32;
		int8_t samples[samplesLength];

		for (uint8_t i = 0; i < samplesLength; i++) {
			error = _SX.getRSSIInst(RSSIF);

			if (error != SX1262Error::none) {
				logSXError("getSpectrumScanningRSSI() error", error);
				stopSpectrumScanning();

				return;
			}

			samples[i] = static_cast<int8_t>(RSSIF);

			// vTaskDelay(pdMS_TO_TICKS(10));
		}

		// RSSI median value
		std::ranges::sort(samples, std::greater<int8_t>());
		const auto RSSI = samples[samplesLength / 2];

		// Adding history record
		const auto newHistoryIndex = std::min<uint64_t>(
			static_cast<uint64_t>(rd.frequency - st.frequency.from)
				* static_cast<uint64_t>(rd.history.size())
				/ static_cast<uint64_t>(st.frequency.to - st.frequency.from),
			rd.history.size() - 1
		);

		if (newHistoryIndex <= _spectrumScanningHistoryIndex) {
			// Accumulating samples
			_spectrumScanningSampleRSSISum += RSSI;
			_spectrumScanningSampleQuantity++;
		}
		else {
			// Computing average values for prev
			_spectrumScanningSampleRSSISum = _spectrumScanningSampleQuantity > 0 ? _spectrumScanningSampleRSSISum / _spectrumScanningSampleQuantity : std::numeric_limits<int8_t>::min();

			// Filling prev
			rd.history[_spectrumScanningHistoryIndex] = _spectrumScanningSampleRSSISum;
			rd.historyMin = std::min<int8_t>(rd.historyMin, _spectrumScanningSampleRSSISum);
			rd.historyMax = std::max<int8_t>(rd.historyMax, _spectrumScanningSampleRSSISum);

			// Clearing what remains between prev & next (for subsequent scans)
			for (uint16_t i = _spectrumScanningHistoryIndex + 1; i < newHistoryIndex; ++i)
				rd.history[i] = std::numeric_limits<int8_t>::min();

			_spectrumScanningHistoryIndex = newHistoryIndex;
			_spectrumScanningSampleRSSISum = RSSI;
			_spectrumScanningSampleQuantity = 1;
		}

		// Moving to next frequency
		rd.frequency += st.frequency.step;

		if (rd.frequency >= st.frequency.to)
			stopSpectrumScanning();
	}

	void RemoteTransceiver::onStart() {
		auto& rc = RC::getInstance();

		while (true) {
			if (rc.getRemoteData().transceiver.spectrumScanning.state == RemoteDataRadioSpectrumScanningState::stopped) {
				transmit(100'000);
				receive(100'000);
			}
			else {
				onSpectrumScanning();
			}
		}
	}
	
	void RemoteTransceiver::onConnectionStateChanged() {
		auto& rc = RC::getInstance();

		if (isConnected()) {
			rc.getAudioPlayer().play(resources::sounds::transceiverConnect);
		}
		else {
			RC::getInstance().getAircraftData().raw.calibration.checkValidTime();
			
			rc.getAudioPlayer().play(resources::sounds::transceiverDisconnect);
		}
	}
	
	
	void RemoteTransceiver::enqueue(const RemotePacketType type) {
		_enqueuedPackets.insert(type);
	}
	
	// -------------------------------- Receiving --------------------------------
	
	bool RemoteTransceiver::onReceive(BitStream& stream, const AircraftPacketType packetType, const uint8_t payloadLength) {
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
	
	bool RemoteTransceiver::receiveAircraftADIRSPacket(BitStream& stream, const uint8_t payloadLength) {
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
		
		auto& rc = RC::getInstance();

		// Roll / pitch / yaw
		rc.getAircraftData().raw.rollRad = readRadians(stream, AircraftADIRSPacket::rollRangeRad, AircraftADIRSPacket::rollLengthBits);
		rc.getAircraftData().raw.pitchRad = readRadians(stream, AircraftADIRSPacket::pitchRangeRad, AircraftADIRSPacket::pitchLengthBits);
		rc.getAircraftData().raw.yawRad = readRadians(stream, AircraftADIRSPacket::yawRangeRad, AircraftADIRSPacket::yawLengthBits);
		
		// Slip & skid
		rc.getAircraftData().raw.slipAndSkidFactor =
			// [0.0, 1.0]
			static_cast<float>(stream.readUint8(AircraftADIRSPacket::slipAndSkidLengthBits))
			/ static_cast<float>((1 << AircraftADIRSPacket::slipAndSkidLengthBits) - 1)
			// [-1.0; 1.0]
			* 2.f - 1.f;
		
		// Speed
		const auto speedFactor =
			static_cast<float>(stream.readUint8(AircraftADIRSPacket::speedLengthBits))
			/ static_cast<float>((1 << AircraftADIRSPacket::speedLengthBits) - 1);
		
		rc.getAircraftData().raw.airspeedMPS = static_cast<float>(AircraftADIRSPacket::speedMaxMPS) * speedFactor;
		
		// Altitude
		rc.getAircraftData().raw.coordinates.setAltitude(readAltitude(
			stream,
			AircraftADIRSPacket::altitudeLengthBits,
			AircraftADIRSPacket::altitudeMinM,
			AircraftADIRSPacket::altitudeMaxM
		));
		
		// Autopilot roll
		rc.getAircraftData().raw.autopilot.rollRad = readRadians(stream, AircraftADIRSPacket::autopilotRollRangeRad, AircraftADIRSPacket::autopilotRollLengthBits);
		
		// Autopilot pitch
		rc.getAircraftData().raw.autopilot.pitchRad = readRadians(stream, AircraftADIRSPacket::autopilotPitchRangeRad, AircraftADIRSPacket::autopilotPitchLengthBits);
		
		// Value conversions
//		rc.getAircraftData().windSpeed = Units::convertSpeed(packet->windSpeedMs, SpeedUnit::meterPerSecond, SpeedUnit::knot);
		
//		rc.getAircraftData().groundSpeedKt = Units::convertSpeed(packet->groundSpeedMs, SpeedUnit::meterPerSecond, SpeedUnit::knot);

//		rc.getAircraftData().flightPathVectorPitch = packet->flightPathPitchRad;
//		rc.getAircraftData().flightPathVectorYaw = packet->flightPathYawRad;
//
//		rc.getAircraftData().flightDirectorPitch = packet->flightDirectorPitchRad;
//		rc.getAircraftData().flightDirectorRoll = packet->flightDirectorRollRad;
//
//		rc.getAircraftData().windDirection = toRadians(packet->windDirectionDeg);
		
		// Trends
		const auto trendsDeltaTime = static_cast<float>(esp_timer_get_time() - _trendsTime);

		if (trendsDeltaTime >= _trendsInterval) {
			// Speed
			rc.getAircraftData().raw.airspeedTrendMPS = (rc.getAircraftData().raw.airspeedMPS - _trendsAirspeedPrevMPS) * 5'000'000.f / trendsDeltaTime;
			_trendsAirspeedPrevMPS = rc.getAircraftData().raw.airspeedMPS;

			// Altitude
			const auto trendsDeltaAltitudeM = rc.getAircraftData().raw.coordinates.getAltitude() - _trendsAltitudePrevM;
			rc.getAircraftData().raw.verticalSpeedMPM = trendsDeltaAltitudeM * 60'000'000.f / trendsDeltaTime;
			rc.getAircraftData().raw.altitudeTrendM = trendsDeltaAltitudeM * 5'000'000.f / trendsDeltaTime;
			_trendsAltitudePrevM = rc.getAircraftData().raw.coordinates.getAltitude();

			_trendsTime = esp_timer_get_time();
		}

		return true;
	}
	
	bool RemoteTransceiver::receiveAircraftAuxiliaryPacket(BitStream& stream, const uint8_t payloadLength) {
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
		
		// -------------------------------- Throttle --------------------------------
		
		rc.getAircraftData().raw.throttle_0_255 =
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
		rc.getAircraftData().raw.coordinates.setLatitude(lat);
		
		// Longitude
		// 26 bits per [0; 360] deg
		// [0.0; 1.0]
		const auto lonFactor =
			static_cast<float>(stream.readUint32(AircraftAuxiliaryPacket::lonLengthBits))
			/ static_cast<float>((1 << AircraftAuxiliaryPacket::lonLengthBits) - 1);
		
		// [-pi; pi]
		const auto lon = lonFactor * std::numbers::pi_v<float> * 2;
		rc.getAircraftData().raw.coordinates.setLongitude(lon);
		
		// -------------------------------- Battery --------------------------------

		// Decavolts
		const auto batteryVoltageDaV = stream.readUint16(AircraftAuxiliaryPacket::batteryLengthBits);
		rc.getAircraftData().raw.batteryVoltageMV = batteryVoltageDaV * 100;
		
		// -------------------------------- Lights --------------------------------
		
		rc.getAircraftData().raw.lights.navigation = stream.readBool();
		rc.getAircraftData().raw.lights.strobe = stream.readBool();
		rc.getAircraftData().raw.lights.landing = stream.readBool();
		rc.getAircraftData().raw.lights.cabin = stream.readBool();
		
		// -------------------------------- Autopilot --------------------------------
		
		// Modes
		rc.getAircraftData().raw.autopilot.lateralMode = static_cast<AutopilotLateralMode>(stream.readUint8(AircraftAuxiliaryPacket::autopilotLateralModeLengthBits));
		rc.getAircraftData().raw.autopilot.verticalMode = static_cast<AutopilotVerticalMode>(stream.readUint8(AircraftAuxiliaryPacket::autopilotVerticalModeLengthBits));
		
		// Altitude for ALT/VNAV modes
		rc.getAircraftData().raw.autopilot.targetAltitudeM = readAltitude(
			stream,
			AircraftAuxiliaryPacket::autopilotAltitudeLengthBits,
			AircraftAuxiliaryPacket::autopilotAltitudeMinM,
			AircraftAuxiliaryPacket::autopilotAltitudeMaxM
		);
		
		// Autothrottle
		rc.getAircraftData().raw.autopilot.autothrottle = stream.readBool();
		
		// Autopilot
		rc.getAircraftData().raw.autopilot.autopilot = stream.readBool();
		
		return true;
	}
	
	bool RemoteTransceiver::receiveAircraftCalibrationPacket(BitStream& stream, const uint8_t payloadLength) {
		if (!validatePayloadChecksumAndLength(
			stream,
			AircraftCalibrationPacket::systemLengthBits
				+ AircraftCalibrationPacket::progressLengthBits,
			payloadLength
		))
			return false;
		
		auto& rc = RC::getInstance();

		rc.getAircraftData().raw.calibration.system = static_cast<AircraftCalibrationSystem>(stream.readUint8(AircraftCalibrationPacket::systemLengthBits));
		rc.getAircraftData().raw.calibration.progress = stream.readUint8(AircraftCalibrationPacket::progressLengthBits) * 0xFF / ((1 << AircraftCalibrationPacket::progressLengthBits) - 1);
		rc.getAircraftData().raw.calibration.setCalibrating(rc.getAircraftData().raw.calibration.progress < 0xFF);
		
//		ESP_LOGI(_logTag, "calibration progress: %d", rc.getAircraftData().raw.calibration.progress * 100 / 0xFF);
		
		return true;
	}
	
	// -------------------------------- Transmitting --------------------------------
	
	RemotePacketType RemoteTransceiver::getTransmitPacketType() {
		const auto& item = _packetSequence[_packetSequenceIndex];
		
		const auto next = [this, &item] {
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
		const auto packetType = item.getType();
			
		next();
			
		return packetType;
	}
	
	bool RemoteTransceiver::onTransmit(BitStream& stream, const RemotePacketType packetType) {
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
	
	void RemoteTransceiver::transmitRemoteControlsPacket(BitStream& stream) {
		auto& rc = RC::getInstance();
		
		// Motors
		
		// Throttle
		const auto writeAxis = [&stream](const uint16_t axisValue) {
			stream.writeUint16(
				static_cast<uint16_t>(axisValue * ((1 << RemoteControlsPacket::motorLengthBits) - 1) / Axis::valueMax),
				RemoteControlsPacket::motorLengthBits
			);
		};

		writeAxis(rc.getAxes().getLeverLeft().getFilteredValue());
		writeAxis(rc.getAxes().getJoystickHorizontal().getFilteredValue());
		writeAxis(rc.getAxes().getJoystickVertical().getFilteredValue());
		writeAxis(rc.getAxes().getRing().getFilteredValue());
		writeAxis(rc.getAxes().getLeverRight().getFilteredValue());
	}
	
	void RemoteTransceiver::transmitRemoteTrimPacket(BitStream& stream) {
		auto& rc = RC::getInstance();
		
		const auto write = [&stream](const int8_t settingsValue) {
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
	
	void RemoteTransceiver::transmitRemoteLightsPacket(BitStream& stream) {
		auto& rc = RC::getInstance();
		
		stream.writeBool(rc.getRemoteData().lights.navigation);
		stream.writeBool(rc.getRemoteData().lights.strobe);
		stream.writeBool(rc.getRemoteData().lights.landing);
		stream.writeBool(rc.getRemoteData().lights.cabin);
	}
	
	void RemoteTransceiver::transmitRemoteBaroPacket(BitStream& stream) {
		auto& rc = RC::getInstance();

		// Reference pressure
		stream.writeUint16(
			rc.getSettings().controls.referencePressureSTD ? 10132 : rc.getSettings().controls.referencePressurePa / 10,
			RemoteBaroPacket::referencePressureLengthBits
		);
	}
	
	void RemoteTransceiver::transmitRemoteAutopilotPacket(BitStream& stream) {
		auto& rc = RC::getInstance();

		// Speed
		const auto speedFactor =
			std::min<float>(
				Units::convertSpeed(rc.getSettings().autopilot.speedKt, SpeedUnit::knot, SpeedUnit::meterPerSecond),
				RemoteAutopilotPacket::speedMaxMPS
			)
			/ static_cast<float>(RemoteAutopilotPacket::speedMaxMPS);
		
		const auto speedMapped = static_cast<float>((1 << RemoteAutopilotPacket::speedLengthBits) - 1) * speedFactor;
		
		stream.writeUint8(static_cast<uint8_t>(std::round(speedMapped)), RemoteAutopilotPacket::speedLengthBits);
		
		// Heading
		stream.writeUint16(rc.getSettings().autopilot.headingDeg, RemoteAutopilotPacket::headingLengthBits);
		
		// Altitude
		const auto altitudeFt =
			rc.getRemoteData().autopilot.verticalMode == AutopilotVerticalMode::alt
			? rc.getRemoteData().autopilot.altitudeHoldFt
			: rc.getSettings().autopilot.altitudeFt;
		
		writeAltitude(
			stream,
			Units::convertDistance(altitudeFt, DistanceUnit::foot, DistanceUnit::meter),
			RemoteAutopilotPacket::altitudeLengthBits,
			RemoteAutopilotPacket::altitudeMinM,
			RemoteAutopilotPacket::altitudeMaxM
		);
		
		// Modes
		stream.writeUint8(std::to_underlying(rc.getRemoteData().autopilot.lateralMode), RemoteAutopilotPacket::lateralModeLengthBits);
		stream.writeUint8(std::to_underlying(rc.getRemoteData().autopilot.verticalMode), RemoteAutopilotPacket::verticalModeLengthBits);
		
		// Autothrottle
		stream.writeBool(rc.getRemoteData().autopilot.autothrottle);
		
		// Autopilot
		stream.writeBool(rc.getRemoteData().autopilot.autopilot);
	}
	
	void RemoteTransceiver::transmitRemoteMotorConfigurationPacket(BitStream& stream) {
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
	
	void RemoteTransceiver::transmitRemoteCalibratePacket(BitStream& stream) {
		auto& rc = RC::getInstance();
		
		stream.writeUint8(std::to_underlying(rc.getRemoteData().calibrationSystem), RemoteCalibratePacket::systemLengthBits);
		
//		ESP_LOGI(_logTag, "Sending calibrate packet");
	}
}
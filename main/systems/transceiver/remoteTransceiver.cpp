#include "systems/transceiver/remoteTransceiver.h"

#include <utility>
#include <algorithm>
#include <limits>

#include <esp_timer.h>
#include <esp_log.h>

#include "rc.h"
#include "resources/sounds.h"

namespace pizda {
	// -------------------------------- Generic --------------------------------

	RemoteTransceiver::RemoteTransceiver() : Transceiver(
		{
			PacketSequenceItem(RemotePacketType::controls, 2),
			PacketSequenceItem(RemotePacketType::controls, 1, true)
		}
	) {

	}

	bool RemoteTransceiver::setup() {
		if (!Transceiver::setup())
			return false;

		_SX.setSPIMutex(RC::getInstance().getSPIMutex());

		return true;
	}

	int8_t RemoteTransceiver::getRSSI() const {
		return _RSSI;
	}

	int8_t RemoteTransceiver::getSNR() const {
		return _SNR;
	}

	void RemoteTransceiver::onStart() {
		auto& rc = RC::getInstance();

		while (true) {
			if (rc.getRemoteData().transceiver.spectrumScanning.state == RemoteDataRadioSpectrumScanningState::stopped) {
				// Should schedule communication settings sync check
				if (_communicationSettingsACKTime < 0) {
					ESP_LOGI(_logTag, "communication settings ACK handled");

					setCommunicationSettings(rc.getRemoteData().transceiver.communicationSettings);

					_communicationSettingsACKTime = esp_timer_get_time() + 2'000'000;
				}
				// Should perform communication settings sync check
				else if (_communicationSettingsACKTime > 0 && esp_timer_get_time() >= _communicationSettingsACKTime) {
					// Received and decoded enough packets to consider the connection is stable
					if (getRXPPS() > 5) {
						ESP_LOGI(_logTag, "communication settings synchronized");

						rc.getSettings().transceiver.communication = rc.getRemoteData().transceiver.communicationSettings;
						rc.getSettings().transceiver.scheduleWrite();
					}
					// Or not enough...
					else {
						ESP_LOGI(_logTag, "communication settings change timed out with PPS = %d, falling back to default", getRXPPS());

						// Falling back to default communication settings
						setCommunicationSettings(config::transceiver::communicationSettings);
					}

					_communicationSettingsACKTime = 0;
				}

				// Transmitting
				if (transmit(100'000)) {

				}

				// Receiving
				if (receive(100'000)) {
					// Updating RSSI and SNR
					if (esp_timer_get_time() > _RSSIAndSNRUpdateTimeUs) {
						float valueF;

						if (_SX.getRSSI(valueF) == SX1262::error::none)
							_RSSI = static_cast<int8_t>(valueF);

						if (_SX.getSNR(valueF) == SX1262::error::none)
							_SNR = static_cast<int8_t>(valueF);

						_RSSIAndSNRUpdateTimeUs = esp_timer_get_time() + _RSSIAndSNRUpdateIntervalUs;
					}
				}
			}
			else {
				onSpectrumScanning();
			}

			PPSTick();
		}
	}
	
	void RemoteTransceiver::onConnectionStateChanged() {
		auto& rc = RC::getInstance();

		if (isConnected()) {
			rc.getAudioPlayer().play(&resources::sounds::transceiverConnect);
		}
		else {
			RC::getInstance().getAircraftData().raw.calibration.checkValidTime();
			
			rc.getAudioPlayer().play(&resources::sounds::transceiverDisconnect);
		}
	}


	bool RemoteTransceiver::stopSpectrumScanning() {
		RC::getInstance().getRemoteData().transceiver.spectrumScanning.state = RemoteDataRadioSpectrumScanningState::stopped;

		// Switching to standby
		auto error = _SX.setStandby();

		if (error != SX1262::error::none) {
			logSXError("stopSpectrumScanning() error", error);
			return false;
		}

		// Setting normal RX/TX frequency
		error = _SX.setRFFrequency(config::transceiver::communicationSettings.frequencyHz);

		if (error != SX1262::error::none) {
			logSXError("stopSpectrumScanning() error", error);
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
			_spectrumScanningSampleCount = 0;

			// Switching to standby
			const auto error = _SX.setStandby();

			if (error != SX1262::error::none) {
				logSXError("onSpectrumScanning() error", error);
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
		if (error != SX1262::error::none) {
			logSXError("onSpectrumScanning() error", error);
			stopSpectrumScanning();

			return;
		}

		vTaskDelay(pdMS_TO_TICKS(10));

		// Moving to RX single mode
		error = _SX.setRX();

		if (error != SX1262::error::none) {
			logSXError("onSpectrumScanning() error", error);
			stopSpectrumScanning();

			return;
		}

		// Applying RSSI inst multisampling
		float RSSIF;
		constexpr static uint8_t samplesLength = 32;
		int8_t samples[samplesLength];

		for (uint8_t i = 0; i < samplesLength; i++) {
			error = _SX.getRSSIInst(RSSIF);

			if (error != SX1262::error::none) {
				logSXError("onSpectrumScanning() error", error);
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
			_spectrumScanningSampleCount++;
		}
		else {
			// Computing average values for prev
			_spectrumScanningSampleRSSISum = _spectrumScanningSampleCount > 0 ? _spectrumScanningSampleRSSISum / _spectrumScanningSampleCount : std::numeric_limits<int8_t>::min();

			// Filling prev
			rd.history[_spectrumScanningHistoryIndex] = _spectrumScanningSampleRSSISum;
			rd.historyMin = std::min<int8_t>(rd.historyMin, _spectrumScanningSampleRSSISum);
			rd.historyMax = std::max<int8_t>(rd.historyMax, _spectrumScanningSampleRSSISum);

			// Clearing what remains between prev & next (for subsequent scans)
			for (uint16_t i = _spectrumScanningHistoryIndex + 1; i < newHistoryIndex; ++i)
				rd.history[i] = std::numeric_limits<int8_t>::min();

			_spectrumScanningHistoryIndex = newHistoryIndex;
			_spectrumScanningSampleRSSISum = RSSI;
			_spectrumScanningSampleCount = 1;
		}

		// Moving to next frequency
		rd.frequency += st.frequency.step;

		if (rd.frequency >= st.frequency.to)
			stopSpectrumScanning();
	}


	// -------------------------------- Receiving --------------------------------
	
	bool RemoteTransceiver::onReceive(BitStream& stream, const AircraftPacketType packetType, const uint8_t payloadLength) {
		RC::getInstance().getAircraftData().raw.calibration.checkValidTime();
		
		switch (packetType) {
			case AircraftPacketType::telemetryPrimary:
				return receiveAircraftTelemetryPrimaryPacket(stream, payloadLength);
			
			case AircraftPacketType::telemetrySecondary:
				return receiveAircraftTelemetrySecondaryPacket(stream, payloadLength);
				
			case AircraftPacketType::auxiliary:
				return receiveAircraftAuxiliaryPacket(stream, payloadLength);
				
			default:
				ESP_LOGE(_logTag, "failed to receive packet: unsupported type %d", packetType);
				
				return false;
		}
	}
	
	bool RemoteTransceiver::receiveAircraftTelemetryPrimaryPacket(BitStream& stream, const uint8_t payloadLength) {
		if (!validatePayloadChecksumAndLength(
			stream,
			AircraftTelemetryPrimaryPacket::rollLengthBits
				+ AircraftTelemetryPrimaryPacket::pitchLengthBits
				+ AircraftTelemetryPrimaryPacket::yawLengthBits
				+ AircraftTelemetryPrimaryPacket::slipAndSkidLengthBits
				+ AircraftTelemetryPrimaryPacket::speedLengthBits
				+ AircraftTelemetryPrimaryPacket::altitudeLengthBits
				+ AircraftTelemetryPrimaryPacket::autopilotRollLengthBits
				+ AircraftTelemetryPrimaryPacket::autopilotPitchLengthBits,
			payloadLength
		))
			return false;
		
		auto& rc = RC::getInstance();

		// Roll / pitch / yaw
		rc.getAircraftData().raw.rollRad = readRadians(stream, AircraftTelemetryPrimaryPacket::rollRangeRad, AircraftTelemetryPrimaryPacket::rollLengthBits);
		rc.getAircraftData().raw.pitchRad = readRadians(stream, AircraftTelemetryPrimaryPacket::pitchRangeRad, AircraftTelemetryPrimaryPacket::pitchLengthBits);
		rc.getAircraftData().raw.yawRad = readRadians(stream, AircraftTelemetryPrimaryPacket::yawRangeRad, AircraftTelemetryPrimaryPacket::yawLengthBits);
		
		// Slip & skid
		rc.getAircraftData().raw.slipAndSkidFactor =
			// [0.0, 1.0]
			static_cast<float>(stream.readUint8(AircraftTelemetryPrimaryPacket::slipAndSkidLengthBits))
			/ static_cast<float>((1 << AircraftTelemetryPrimaryPacket::slipAndSkidLengthBits) - 1)
			// [-1.0; 1.0]
			* 2.f - 1.f;
		
		// Speed
		const auto speedFactor =
			static_cast<float>(stream.readUint8(AircraftTelemetryPrimaryPacket::speedLengthBits))
			/ static_cast<float>((1 << AircraftTelemetryPrimaryPacket::speedLengthBits) - 1);
		
		rc.getAircraftData().raw.airspeedMPS = static_cast<float>(AircraftTelemetryPrimaryPacket::speedMaxMPS) * speedFactor;
		
		// Altitude
		rc.getAircraftData().raw.coordinates.setAltitude(readAltitude(
			stream,
			AircraftTelemetryPrimaryPacket::altitudeLengthBits,
			AircraftTelemetryPrimaryPacket::altitudeMinM,
			AircraftTelemetryPrimaryPacket::altitudeMaxM
		));
		
		// Autopilot roll
		rc.getAircraftData().raw.autopilot.rollRad = readRadians(stream, AircraftTelemetryPrimaryPacket::autopilotRollRangeRad, AircraftTelemetryPrimaryPacket::autopilotRollLengthBits);
		
		// Autopilot pitch
		rc.getAircraftData().raw.autopilot.pitchRad = readRadians(stream, AircraftTelemetryPrimaryPacket::autopilotPitchRangeRad, AircraftTelemetryPrimaryPacket::autopilotPitchLengthBits);
		
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

		if (trendsDeltaTime >= trendsInterval) {
			// Yaw
			rc.getAircraftData().raw.yawTrendDeg = -toDegrees(rc.getAircraftData().raw.yawRad - _trendsYawPrevRad) * 2'500'000.f / trendsDeltaTime;
			_trendsYawPrevRad = rc.getAircraftData().raw.yawRad;

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
	
	bool RemoteTransceiver::receiveAircraftTelemetrySecondaryPacket(BitStream& stream, const uint8_t payloadLength) {
		if (!validatePayloadChecksumAndLength(
			stream,
			AircraftTelemetrySecondaryPacket::throttleLengthBits
				+ AircraftTelemetrySecondaryPacket::latLengthBits
				+ AircraftTelemetrySecondaryPacket::lonLengthBits
				+ AircraftTelemetrySecondaryPacket::batteryLengthBits
				// Lights
				+ 4
				+ AircraftTelemetrySecondaryPacket::autopilotLateralModeLengthBits
				+ AircraftTelemetrySecondaryPacket::autopilotVerticalModeLengthBits
				+ AircraftTelemetrySecondaryPacket::autopilotAltitudeLengthBits
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
			stream.readUint8(AircraftTelemetrySecondaryPacket::throttleLengthBits)
			* 0xFF
			/ ((1 << AircraftTelemetrySecondaryPacket::throttleLengthBits) - 1);
		
		// -------------------------------- Latitude & longitude --------------------------------
		
		// Latitude
		// 25 bits per [-90; 90] deg
		// [0.0; 1.0]
		const auto latFactor =
			static_cast<float>(stream.readUint32(AircraftTelemetrySecondaryPacket::latLengthBits))
			/ static_cast<float>((1 << AircraftTelemetrySecondaryPacket::latLengthBits) - 1);
		
		// [-pi / 2; pi / 2]
		const auto lat = latFactor * std::numbers::pi_v<float> - std::numbers::pi_v<float> / 2.f;
		rc.getAircraftData().raw.coordinates.setLatitude(lat);
		
		// Longitude
		// 26 bits per [0; 360] deg
		// [0.0; 1.0]
		const auto lonFactor =
			static_cast<float>(stream.readUint32(AircraftTelemetrySecondaryPacket::lonLengthBits))
			/ static_cast<float>((1 << AircraftTelemetrySecondaryPacket::lonLengthBits) - 1);
		
		// [-pi; pi]
		const auto lon = lonFactor * std::numbers::pi_v<float> * 2;
		rc.getAircraftData().raw.coordinates.setLongitude(lon);
		
		// -------------------------------- Battery --------------------------------

		// Decavolts
		const auto batteryVoltageDaV = stream.readUint16(AircraftTelemetrySecondaryPacket::batteryLengthBits);
		rc.getAircraftData().raw.batteryVoltageMV = batteryVoltageDaV * 100;
		
		// -------------------------------- Lights --------------------------------
		
		rc.getAircraftData().raw.lights.navigation = stream.readBool();
		rc.getAircraftData().raw.lights.strobe = stream.readBool();
		rc.getAircraftData().raw.lights.landing = stream.readBool();
		rc.getAircraftData().raw.lights.cabin = stream.readBool();
		
		// -------------------------------- Autopilot --------------------------------
		
		// Modes
		rc.getAircraftData().raw.autopilot.lateralMode = static_cast<AutopilotLateralMode>(stream.readUint8(AircraftTelemetrySecondaryPacket::autopilotLateralModeLengthBits));
		rc.getAircraftData().raw.autopilot.verticalMode = static_cast<AutopilotVerticalMode>(stream.readUint8(AircraftTelemetrySecondaryPacket::autopilotVerticalModeLengthBits));
		
		// Altitude for ALT/VNAV modes
		rc.getAircraftData().raw.autopilot.targetAltitudeM = readAltitude(
			stream,
			AircraftTelemetrySecondaryPacket::autopilotAltitudeLengthBits,
			AircraftTelemetrySecondaryPacket::autopilotAltitudeMinM,
			AircraftTelemetrySecondaryPacket::autopilotAltitudeMaxM
		);
		
		// Autothrottle
		rc.getAircraftData().raw.autopilot.autothrottle = stream.readBool();
		
		// Autopilot
		rc.getAircraftData().raw.autopilot.autopilot = stream.readBool();

		// Gyro
		rc.getAircraftData().raw.autopilot.gyro = stream.readBool();
		
		return true;
	}

	bool RemoteTransceiver::receiveAircraftAuxiliaryPacket(BitStream& stream, const uint8_t payloadLength) {
		const auto type = static_cast<AircraftAuxiliaryPacketType>(stream.readUint8(AircraftAuxiliaryPacket::typeLengthBits));

		switch (type) {
			case AircraftAuxiliaryPacketType::calibration:
				receiveAircraftAuxiliaryCalibrationPacket(stream, payloadLength);
				break;
			case AircraftAuxiliaryPacketType::XCVRACK:
				receiveAircraftAuxiliaryXCVRACKPacket(stream, payloadLength);
				break;
			default:
				break;
		}

		return true;
	}
	
	bool RemoteTransceiver::receiveAircraftAuxiliaryCalibrationPacket(BitStream& stream, const uint8_t payloadLength) {
		if (!validatePayloadChecksumAndLength(
			stream,
			AircraftAuxiliaryPacket::typeLengthBits
				+ AircraftAuxiliaryCalibrationPacket::systemLengthBits
				+ AircraftAuxiliaryCalibrationPacket::progressLengthBits,
			payloadLength
		))
			return false;
		
		auto& rc = RC::getInstance();

		rc.getAircraftData().raw.calibration.system = static_cast<AircraftCalibrationSystem>(stream.readUint8(AircraftAuxiliaryCalibrationPacket::systemLengthBits));
		rc.getAircraftData().raw.calibration.progress = static_cast<uint16_t>(stream.readUint8(AircraftAuxiliaryCalibrationPacket::progressLengthBits)) * 0xFF / ((1 << AircraftAuxiliaryCalibrationPacket::progressLengthBits) - 1);
		rc.getAircraftData().raw.calibration.setCalibrating(rc.getAircraftData().raw.calibration.progress < 0xFF);
		
//		ESP_LOGI(_logTag, "calibration progress: %d", rc.getAircraftData().raw.calibration.progress * 100 / 0xFF);
		
		return true;
	}

	bool RemoteTransceiver::receiveAircraftAuxiliaryXCVRACKPacket(BitStream& stream, const uint8_t payloadLength) {
		if (!validatePayloadChecksumAndLength(
			stream,
			AircraftAuxiliaryPacket::typeLengthBits,
			payloadLength
		))
			return false;

		_communicationSettingsACKTime = -1;

		return true;
	}
	
	// -------------------------------- Transmitting --------------------------------

	void RemoteTransceiver::onTransmit(BitStream& stream, const RemotePacketType packetType) {
		switch (packetType) {
			case RemotePacketType::controls:
				transmitRemoteControlsPacket(stream);
				break;

			case RemotePacketType::auxiliary:
				transmitRemoteAuxiliaryPacket(stream);
				break;
		}
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

	void RemoteTransceiver::transmitRemoteAuxiliaryPacket(BitStream& stream) {
		// Type
		stream.writeUint8(std::to_underlying(getEnqueuedAuxiliaryPacketType()), RemoteAuxiliaryPacket::typeLengthBits);

		switch (getEnqueuedAuxiliaryPacketType()) {
			case RemoteAuxiliaryPacketType::trim:
				transmitRemoteAuxiliaryTrimPacket(stream);
				break;

			case RemoteAuxiliaryPacketType::lights:
				transmitRemoteAuxiliaryLightsPacket(stream);
				break;

			case RemoteAuxiliaryPacketType::baro:
				transmitRemoteAuxiliaryBaroPacket(stream);
				break;

			case RemoteAuxiliaryPacketType::autopilot:
				transmitRemoteAuxiliaryAutopilotPacket(stream);
				break;

			case RemoteAuxiliaryPacketType::motors:
				transmitRemoteAuxiliaryMotorsPacket(stream);
				break;

			case RemoteAuxiliaryPacketType::calibrate:
				transmitRemoteAuxiliaryCalibratePacket(stream);
				break;

			case RemoteAuxiliaryPacketType::ADIRS:
				transmitRemoteAuxiliaryADIRSPacket(stream);
				break;

			case RemoteAuxiliaryPacketType::XCVR:
				transmitRemoteAuxiliaryXCVRPacket(stream);
				break;

			case RemoteAuxiliaryPacketType::PID:
				transmitRemoteAuxiliaryPIDPacket(stream);
				break;

			default:
				ESP_LOGE(_logTag, "failed to transmit packet: unsupported type %d", getEnqueuedAuxiliaryPacketType());
				break;
		}
	}
	
	void RemoteTransceiver::transmitRemoteAuxiliaryTrimPacket(BitStream& stream) {
		auto& rc = RC::getInstance();
		
		const auto write = [&stream](const int8_t settingsValue) {
			stream.writeUint16(
				// Mapping from [-100; 100] to [0; bits]
				static_cast<uint16_t>(
					(static_cast<int32_t>(settingsValue) + 100)
					* ((1 << RemoteAuxiliaryTrimPacket::valueLengthBits) - 1)
					/ 200
				),
				RemoteAuxiliaryTrimPacket::valueLengthBits
			);
		};
		
		write(rc.getSettings().controls.aileronsTrim);
		write(rc.getSettings().controls.elevatorTrim);
		write(rc.getSettings().controls.rudderTrim);
	}
	
	void RemoteTransceiver::transmitRemoteAuxiliaryLightsPacket(BitStream& stream) {
		auto& rc = RC::getInstance();
		
		stream.writeBool(rc.getRemoteData().lights.navigation);
		stream.writeBool(rc.getRemoteData().lights.strobe);
		stream.writeBool(rc.getRemoteData().lights.landing);
		stream.writeBool(rc.getRemoteData().lights.cabin);
	}
	
	void RemoteTransceiver::transmitRemoteAuxiliaryBaroPacket(BitStream& stream) {
		auto& rc = RC::getInstance();

		// Reference pressure
		stream.writeUint16(
			rc.getSettings().ADIRS.referencePressureSTD ? 10132 : rc.getSettings().ADIRS.referencePressurePa / 10,
			RemoteAuxiliaryBaroPacket::referencePressureLengthBits
		);
	}
	
	void RemoteTransceiver::transmitRemoteAuxiliaryAutopilotPacket(BitStream& stream) {
		auto& rc = RC::getInstance();

		// Speed
		const auto speedFactor =
			std::min<float>(
				Units::convertSpeed(rc.getSettings().autopilot.speedKt, SpeedUnit::knot, SpeedUnit::meterPerSecond),
				RemoteAuxiliaryAutopilotPacket::speedMaxMPS
			)
			/ static_cast<float>(RemoteAuxiliaryAutopilotPacket::speedMaxMPS);
		
		const auto speedMapped = static_cast<float>((1 << RemoteAuxiliaryAutopilotPacket::speedLengthBits) - 1) * speedFactor;
		
		stream.writeUint8(static_cast<uint8_t>(std::round(speedMapped)), RemoteAuxiliaryAutopilotPacket::speedLengthBits);
		
		// Heading
		stream.writeUint16(rc.getSettings().autopilot.headingDeg, RemoteAuxiliaryAutopilotPacket::headingLengthBits);
		
		// Altitude
		const auto altitudeFt =
			rc.getRemoteData().autopilot.verticalMode == AutopilotVerticalMode::alt
			? rc.getRemoteData().autopilot.altitudeHoldFt
			: rc.getSettings().autopilot.altitudeFt;
		
		writeAltitude(
			stream,
			Units::convertDistance(altitudeFt, DistanceUnit::foot, DistanceUnit::meter),
			RemoteAuxiliaryAutopilotPacket::altitudeLengthBits,
			RemoteAuxiliaryAutopilotPacket::altitudeMinM,
			RemoteAuxiliaryAutopilotPacket::altitudeMaxM
		);
		
		// Modes
		stream.writeUint8(std::to_underlying(rc.getRemoteData().autopilot.lateralMode), RemoteAuxiliaryAutopilotPacket::lateralModeLengthBits);
		stream.writeUint8(std::to_underlying(rc.getRemoteData().autopilot.verticalMode), RemoteAuxiliaryAutopilotPacket::verticalModeLengthBits);
		
		// Autothrottle
		stream.writeBool(rc.getRemoteData().autopilot.autothrottle);
		
		// Autopilot
		stream.writeBool(rc.getRemoteData().autopilot.autopilot);

		// Gyro
		stream.writeBool(rc.getRemoteData().autopilot.gyro);
	}
	
	void RemoteTransceiver::transmitRemoteAuxiliaryMotorsPacket(BitStream& stream) {
		const auto& motors = RC::getInstance().getSettings().motors;
	
		const auto write = [&stream](const MotorSettings& motor) {
			stream.writeUint16(motor.min, RemoteAuxiliaryMotorConfigurationPacket::minLengthBits);
			stream.writeUint16(motor.max, RemoteAuxiliaryMotorConfigurationPacket::maxLengthBits);
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
	
	void RemoteTransceiver::transmitRemoteAuxiliaryCalibratePacket(BitStream& stream) {
		auto& rc = RC::getInstance();
		
		stream.writeUint8(std::to_underlying(rc.getRemoteData().calibrationSystem), RemoteAuxiliaryCalibratePacket::systemLengthBits);

//		ESP_LOGI(_logTag, "Sending calibrate packet");
	}

	void RemoteTransceiver::transmitRemoteAuxiliaryADIRSPacket(BitStream& stream) {
		auto& rc = RC::getInstance();

		stream.writeInt16(rc.getSettings().ADIRS.magneticDeclinationDeg, RemoteAuxiliaryADIRSPacket::magneticDeclinationLengthBits);
	}

	void RemoteTransceiver::transmitRemoteAuxiliaryXCVRPacket(BitStream& stream) {
		const auto& settings = RC::getInstance().getRemoteData().transceiver.communicationSettings;

		ESP_LOGI(_logTag, "transmitting communication settings");

		stream.writeUint16(settings.frequencyHz / 1'000'000, RemoteAuxiliaryXCVRPacket::RFFrequencyLengthBits);
		stream.writeUint8(std::to_underlying(settings.bandwidth), RemoteAuxiliaryXCVRPacket::bandwidthLengthBits);
		stream.writeUint8(settings.spreadingFactor, RemoteAuxiliaryXCVRPacket::spreadingFactorLengthBits);
		stream.writeUint8(std::to_underlying(settings.codingRate), RemoteAuxiliaryXCVRPacket::codingRateLengthBits);
		stream.writeUint8(settings.syncWord, RemoteAuxiliaryXCVRPacket::syncWordLengthBits);
		stream.writeInt8(settings.powerDBm, RemoteAuxiliaryXCVRPacket::powerDBmLengthBits);
		stream.writeUint16(settings.preambleLength, RemoteAuxiliaryXCVRPacket::preambleLengthLengthBits);
	}

	void RemoteTransceiver::transmitRemoteAuxiliaryPIDPacket(BitStream& stream) {
		const auto& data = RC::getInstance().getRemoteData().autopilot.PID;

		ESP_LOGI(_logTag, "TX PID packet, type: %d, P: %f, I: %f, D: %f", data.type, data.coefficients.p, data.coefficients.i, data.coefficients.d);

		stream.writeUint8(std::to_underlying(data.type), RemoteAuxiliaryPIDPacket::typeLengthBits);
		stream.writeFloat(data.coefficients.p);
		stream.writeFloat(data.coefficients.i);
		stream.writeFloat(data.coefficients.d);
	}
}
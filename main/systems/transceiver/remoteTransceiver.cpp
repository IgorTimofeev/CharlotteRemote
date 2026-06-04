#include "systems/transceiver/remoteTransceiver.h"

#include <utility>
#include <algorithm>
#include <limits>

#include <esp_timer.h>
#include <esp_log.h>

#include "rc.h"
#include "resources/sounds.h"

namespace pizda {
	// -------------------------------- Main --------------------------------

	RemoteTransceiver::RemoteTransceiver() : Transceiver(
		{
			PacketSequenceItem(RemotePacketType::controls),
			PacketSequenceItem(RemotePacketType::controls),
			PacketSequenceItem(RemotePacketType::controls, true)
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

	void RemoteTransceiver::onTick() {
		auto& rc = RC::getInstance();

		// Anything related directly to XCVR module should be processed in single loop, BECAUSE WE NEED FULL AUTHORITY
		switch (rc.getRemoteData().transceiver.spectrumScanning.state) {
			case RemoteDataRadioSpectrumScanningState::stopped: {
				// Communication settings ACK packet received
				if (_communicationSettingsACKTime < 0) {
					scheduleCommunicationSettingsSyncCheck();
				}
				// Communication settings sync check time reached
				else if (_communicationSettingsACKTime > 0 && esp_timer_get_time() >= _communicationSettingsACKTime) {
					performCommunicationSettingsSyncCheck();
				}

				// Transmitting
				if (transmit(100'000)) {

				}

				// Receiving
				if (receive(100'000)) {
					updateRSSIAndSNR();
				}

				break;
			}
			default: {
				spectrumScanningTick();

				break;
			}
		}
	}

	void RemoteTransceiver::scheduleCommunicationSettingsSyncCheck() {
		ESP_LOGI(_logTag, "communication settings ACK received");

		setCommunicationSettings(RC::getInstance().getRemoteData().transceiver.communicationSettings);

		_communicationSettingsACKTime = esp_timer_get_time() + _communicationSettingsACKDelayUs;
	}

	void RemoteTransceiver::performCommunicationSettingsSyncCheck() {
		auto& rc = RC::getInstance();

		// Received and decoded enough packets to consider the connection is stable
		if (getRXPPS() >= _communicationSettingsACKMinValidPPS) {
			ESP_LOGI(_logTag, "communication settings synchronized");

			rc.getSettings().transceiver.communication = rc.getRemoteData().transceiver.communicationSettings;
			rc.getSettings().transceiver.scheduleWrite();
		}
		// Or not enough...
		else {
			ESP_LOGI(_logTag, "communication settings change timed out with PPS = %d, falling back to default", getRXPPS());

			// Falling back to default communication settings
			setCommunicationSettings(config::XCVR::communicationSettings);
		}

		_communicationSettingsACKTime = 0;
	}

	void RemoteTransceiver::updateRSSIAndSNR() {
		if (esp_timer_get_time() < _RSSIAndSNRUpdateTimeUs)
			return;

		float valueF;

		if (_SX.getRSSI(valueF) == SX1262::error::none)
			_RSSI = static_cast<int8_t>(valueF);

		if (_SX.getSNR(valueF) == SX1262::error::none)
			_SNR = static_cast<int8_t>(valueF);

		_RSSIAndSNRUpdateTimeUs = esp_timer_get_time() + _RSSIAndSNRUpdateIntervalUs;
	}

	void RemoteTransceiver::onConnectionStateChanged() {
		auto& rc = RC::getInstance();

		switch (getConnectionState()) {
			case ConnectionState::initial:
				break;

			case ConnectionState::connected:
				// WHOOP WHOOP TERRAIN AHEAD
				rc.getAudioPlayer().play(&resources::sounds::transceiverConnect);
				break;

			case ConnectionState::disconnected:
				RC::getInstance().getAircraftData().raw.calibration.checkValidTime();

				rc.getAudioPlayer().play(&resources::sounds::transceiverDisconnect);

				break;

			case ConnectionState::reconnected:
				rc.getAudioPlayer().play(&resources::sounds::transceiverConnect);

				break;
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
		error = _SX.setRFFrequency(config::XCVR::communicationSettings.frequencyHz);

		if (error != SX1262::error::none) {
			logSXError("stopSpectrumScanning() error", error);
			return false;
		}

		return true;
	}

	void RemoteTransceiver::spectrumScanningTick() {
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
		constexpr static uint8_t RSSISamplesLength = 16;
		int8_t RSSISamples[RSSISamplesLength];

		for (uint8_t i = 0; i < RSSISamplesLength; i++) {
			error = _SX.getRSSIInst(RSSIF);

			if (error != SX1262::error::none) {
				logSXError("onSpectrumScanning() error", error);
				stopSpectrumScanning();

				return;
			}

			RSSISamples[i] = static_cast<int8_t>(RSSIF);

			// vTaskDelay(pdMS_TO_TICKS(10));
		}

		// RSSI median value
		std::ranges::sort(RSSISamples, std::greater<int8_t>());
		const auto RSSI = RSSISamples[RSSISamplesLength / 2];

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
			case AircraftPacketType::STierTelemetry: {
				if (!validatePayloadChecksumAndLength(
					stream,
					AircraftSTierTelemetryPacket::rollLengthBits
						+ AircraftSTierTelemetryPacket::pitchLengthBits
						+ AircraftSTierTelemetryPacket::yawLengthBits

						+ AircraftSTierTelemetryPacket::slipAndSkidLengthBits
						+ AircraftSTierTelemetryPacket::speedLengthBits
						+ AircraftSTierTelemetryPacket::altitudeLengthBits

						+ AircraftSTierTelemetryPacket::autopilotTargetRollLengthBits
						+ AircraftSTierTelemetryPacket::autopilotTargetPitchLengthBits,
					payloadLength
				))
					return false;

				auto& rc = RC::getInstance();

				// -------------------------------- Roll / pitch / yaw --------------------------------

				rc.getAircraftData().raw.rollRad = readRadians(stream, AircraftSTierTelemetryPacket::rollRangeRad, AircraftSTierTelemetryPacket::rollLengthBits);
				rc.getAircraftData().raw.pitchRad = readRadians(stream, AircraftSTierTelemetryPacket::pitchRangeRad, AircraftSTierTelemetryPacket::pitchLengthBits);
				rc.getAircraftData().raw.yawRad = readRadians(stream, AircraftSTierTelemetryPacket::yawRangeRad, AircraftSTierTelemetryPacket::yawLengthBits);

				// -------------------------------- Slip & skid --------------------------------

				rc.getAircraftData().raw.slipAndSkidFactor =
					// [0.0, 1.0]
					static_cast<float>(stream.readUint8(AircraftSTierTelemetryPacket::slipAndSkidLengthBits))
					/ static_cast<float>((1 << AircraftSTierTelemetryPacket::slipAndSkidLengthBits) - 1)
					// [-1.0; 1.0]
					* 2.f - 1.f;

				// -------------------------------- Speed --------------------------------

				const auto speedFactor =
					static_cast<float>(stream.readUint8(AircraftSTierTelemetryPacket::speedLengthBits))
					/ static_cast<float>((1 << AircraftSTierTelemetryPacket::speedLengthBits) - 1);

				rc.getAircraftData().raw.airspeedMPS = static_cast<float>(AircraftSTierTelemetryPacket::speedMaxMPS) * speedFactor;

				// -------------------------------- Altitude --------------------------------

				rc.getAircraftData().raw.coordinates.setAltitude(readAltitude(
					stream,
					AircraftSTierTelemetryPacket::altitudeLengthBits,
					AircraftSTierTelemetryPacket::altitudeMinM,
					AircraftSTierTelemetryPacket::altitudeMaxM
				));

				// -------------------------------- Autopilot target roll / pitch --------------------------------

				rc.getAircraftData().raw.autopilot.rollRad = readRadians(stream, AircraftSTierTelemetryPacket::autopilotTargetRollLengthBits, AircraftSTierTelemetryPacket::autopilotTargetRollLengthBits);
				rc.getAircraftData().raw.autopilot.pitchRad = readRadians(stream, AircraftSTierTelemetryPacket::autopilotTargetPitchLengthBits, AircraftSTierTelemetryPacket::autopilotTargetPitchLengthBits);

				// -------------------------------- Data processing --------------------------------

				// Trends
				const auto trendsDeltaTime = static_cast<float>(esp_timer_get_time() - _trendsTime);

				if (trendsDeltaTime >= trendsInterval) {
					// Yaw
					rc.getAircraftData().raw.yawTrendDeg = -toDegrees(rc.getAircraftData().raw.yawRad - _trendsYawPrevRad) * 5'000'000.f / trendsDeltaTime;
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
			case AircraftPacketType::ATierTelemetry: {
				if (!validatePayloadChecksumAndLength(
					stream,
						// Throttle
						AircraftATierTelemetryPacket::throttleLengthBits
						// Latitude & longitude
						+ AircraftATierTelemetryPacket::latitudeLengthBits
						+ AircraftATierTelemetryPacket::longitudeLengthBits,
					payloadLength
				))
					return false;

				auto& rc = RC::getInstance();

				// -------------------------------- Throttle --------------------------------

				rc.getAircraftData().raw.throttle_0_255 =
					stream.readUint8(AircraftATierTelemetryPacket::throttleLengthBits)
					* 0xFF
					/ ((1 << AircraftATierTelemetryPacket::throttleLengthBits) - 1);

				// -------------------------------- Latitude & longitude --------------------------------

				rc.getAircraftData().raw.coordinates.setLatitude(readLatitude(stream, AircraftATierTelemetryPacket::latitudeLengthBits));
				rc.getAircraftData().raw.coordinates.setLongitude(readLongitude(stream, AircraftATierTelemetryPacket::longitudeLengthBits));

				return true;
			}
			case AircraftPacketType::BTierTelemetry: {
				if (!validatePayloadChecksumAndLength(
					stream,
						// Autopilot
						AircraftBTierTelemetryPacket::autopilotLateralModeLengthBits
						+ AircraftBTierTelemetryPacket::autopilotVerticalModeLengthBits
						+ AircraftBTierTelemetryPacket::autopilotAltitudeLengthBits
						// A/T
						+ 1
						// A/P
						+ 1
						// Lights
						+ 4
						// Battery
						+ AircraftBTierTelemetryPacket::batteryLengthBits,

					payloadLength
				))
					return false;

				auto& rc = RC::getInstance();

				// -------------------------------- Autopilot --------------------------------

				// Modes
				rc.getAircraftData().raw.autopilot.lateralMode = static_cast<AutopilotLateralMode>(stream.readUint8(AircraftBTierTelemetryPacket::autopilotLateralModeLengthBits));
				rc.getAircraftData().raw.autopilot.verticalMode = static_cast<AutopilotVerticalMode>(stream.readUint8(AircraftBTierTelemetryPacket::autopilotVerticalModeLengthBits));

				// Altitude for ALT/VNAV modes
				rc.getAircraftData().raw.autopilot.targetAltitudeM = readAltitude(
					stream,
					AircraftBTierTelemetryPacket::autopilotAltitudeLengthBits,
					AircraftBTierTelemetryPacket::autopilotAltitudeMinM,
					AircraftBTierTelemetryPacket::autopilotAltitudeMaxM
				);

				// Autothrottle
				rc.getAircraftData().raw.autopilot.autothrottle = stream.readBool();

				// Autopilot
				rc.getAircraftData().raw.autopilot.autopilot = stream.readBool();

				// -------------------------------- Lights --------------------------------

				rc.getAircraftData().raw.lights.navigation = stream.readBool();
				rc.getAircraftData().raw.lights.strobe = stream.readBool();
				rc.getAircraftData().raw.lights.landing = stream.readBool();
				rc.getAircraftData().raw.lights.cabin = stream.readBool();

				// -------------------------------- Battery --------------------------------

				// Decavolts
				const auto batteryVoltageDaV = stream.readUint16(AircraftBTierTelemetryPacket::batteryLengthBits);
				rc.getAircraftData().raw.batteryVoltageMV = batteryVoltageDaV * 100;

				return true;
			}
			case AircraftPacketType::system: {
				const auto aircraftSystemPacketType = static_cast<AircraftSystemPacketType>(stream.readUint8(AircraftSystemPacket::typeLengthBits));

				switch (aircraftSystemPacketType) {
					case AircraftSystemPacketType::calibration: {
						if (!validatePayloadChecksumAndLength(
							stream,
							AircraftSystemPacket::typeLengthBits
								+ AircraftSystemCalibrationPacket::systemLengthBits
								+ AircraftSystemCalibrationPacket::progressLengthBits,
							payloadLength
						))
							return false;

						auto& rc = RC::getInstance();

						rc.getAircraftData().raw.calibration.system = static_cast<AircraftCalibrationSystem>(stream.readUint8(AircraftSystemCalibrationPacket::systemLengthBits));
						rc.getAircraftData().raw.calibration.progress = static_cast<uint16_t>(stream.readUint8(AircraftSystemCalibrationPacket::progressLengthBits)) * 0xFF / ((1 << AircraftSystemCalibrationPacket::progressLengthBits) - 1);
						rc.getAircraftData().raw.calibration.setCalibrating(rc.getAircraftData().raw.calibration.progress < 0xFF);

						//		ESP_LOGI(_logTag, "calibration progress: %d", rc.getAircraftData().raw.calibration.progress * 100 / 0xFF);

						return true;
					}
					case AircraftSystemPacketType::communicationSettingsACK: {
						if (!validatePayloadChecksumAndLength(
							stream,
							AircraftSystemPacket::typeLengthBits,
							payloadLength
						))
							return false;

						_communicationSettingsACKTime = -1;

						return true;
					}
					default: {
						ESP_LOGE(_logTag, "failed to receive packet: unsupported aircraft system packet type %d", aircraftSystemPacketType);

						return false;
					}
				}
			}
			default: {
				ESP_LOGE(_logTag, "failed to receive packet: unsupported type %d", packetType);

				return false;
			}
		}
	}

	// -------------------------------- Transmitting --------------------------------

	void RemoteTransceiver::onTransmit(BitStream& stream, const RemotePacketType packetType) {
		auto& rc = RC::getInstance();

		switch (packetType) {
			case RemotePacketType::controls: {
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

				break;
			}
			case RemotePacketType::system: {
				transmitRemoteSystemPacket(stream);

				break;
			}
		}
	}

	void RemoteTransceiver::transmitRemoteSystemPacket(BitStream& stream) const {
		auto& rc = RC::getInstance();

		// Type
		stream.writeUint8(std::to_underlying(getEnqueuedSystemPacketType()), RemoteSystemPacket::typeLengthBits);
		
		switch (getEnqueuedSystemPacketType()) {
			case RemoteSystemPacketType::trim: {
				const auto write = [&stream](const int8_t settingsValue) {
					stream.writeUint16(
						// Mapping from [-100; 100] to [0; bits]
						static_cast<uint16_t>(
							(static_cast<int32_t>(settingsValue) + 100)
							* ((1 << RemoteSystemPacket::trimValueLengthBits) - 1)
							/ 200
						),
						RemoteSystemPacket::trimValueLengthBits
					);
				};

				write(rc.getSettings().controls.aileronsTrim);
				write(rc.getSettings().controls.elevatorTrim);
				write(rc.getSettings().controls.rudderTrim);

				break;
			}
			case RemoteSystemPacketType::lights: {
				stream.writeBool(rc.getRemoteData().lights.navigation);
				stream.writeBool(rc.getRemoteData().lights.strobe);
				stream.writeBool(rc.getRemoteData().lights.landing);
				stream.writeBool(rc.getRemoteData().lights.cabin);

				break;
			}
			case RemoteSystemPacketType::referencePressure: {
				stream.writeUint16(
					rc.getSettings().ADIRS.referencePressureSTD ? 10132 : rc.getSettings().ADIRS.referencePressurePa / 10,
					RemoteSystemPacket::referencePressureLengthBits
				);

				break;
			}
			case RemoteSystemPacketType::camera: {
				stream.writeInt16(rc.getSettings().controls.cameraPitchDeg, RemoteSystemPacket::cameraPitchLengthBits);
				stream.writeInt16(rc.getSettings().controls.cameraYawDeg, RemoteSystemPacket::cameraYawLengthBits);

				break;
			}
			case RemoteSystemPacketType::motors: {
				const auto& motor = RC::getInstance().getRemoteData().motorSettings;

				stream.writeUint8(static_cast<uint8_t>(motor.type), RemoteSystemPacket::motorConfigurationTypeLengthBits);
				stream.writeUint16(motor.settings.min, RemoteSystemPacket::motorConfigurationMinLengthBits);
				stream.writeUint16(motor.settings.max, RemoteSystemPacket::motorConfigurationMaxLengthBits);
				stream.writeBool(motor.settings.reverse);

				break;
			}
			case RemoteSystemPacketType::calibrate: {
				stream.writeUint8(std::to_underlying(rc.getRemoteData().calibrationSystem), RemoteSystemPacket::calibrateSystemLengthBits);

				//		ESP_LOGI(_logTag, "Sending calibrate packet");

				break;
			}
			case RemoteSystemPacketType::communicationSettings: {
				const auto& settings = RC::getInstance().getRemoteData().transceiver.communicationSettings;

				ESP_LOGI(_logTag, "transmitting communication settings");

				stream.writeUint16(settings.frequencyHz / 1'000'000, RemoteSystemCommunicationSettingsPacket::RFFrequencyLengthBits);
				stream.writeUint8(std::to_underlying(settings.bandwidth), RemoteSystemCommunicationSettingsPacket::bandwidthLengthBits);
				stream.writeUint8(settings.spreadingFactor, RemoteSystemCommunicationSettingsPacket::spreadingFactorLengthBits);
				stream.writeUint8(std::to_underlying(settings.codingRate), RemoteSystemCommunicationSettingsPacket::codingRateLengthBits);
				stream.writeUint8(settings.syncWord, RemoteSystemCommunicationSettingsPacket::syncWordLengthBits);
				stream.writeUint16(settings.preambleLength, RemoteSystemCommunicationSettingsPacket::preambleLengthLengthBits);

				stream.writeInt8(settings.currentLimitMA, RemoteSystemCommunicationSettingsPacket::currentLimitMALengthBits);
				stream.writeInt8(settings.powerDBm, RemoteSystemCommunicationSettingsPacket::powerDBmLengthBits);

				break;
			}
			case RemoteSystemPacketType::magneticDeclination: {
				stream.writeInt16(rc.getSettings().ADIRS.magneticDeclinationDeg, RemoteSystemPacket::magneticDeclinationLengthBits);

				break;
			}
			case RemoteSystemPacketType::homeCoordinates: {
				writeLatitude(stream, rc.getRemoteData().homeCoordinates.getLatitude(), RemoteSystemPacket::homeCoordinatesLatitudeLengthBits);
				writeLongitude(stream, rc.getRemoteData().homeCoordinates.getLongitude(), RemoteSystemPacket::homeCoordinatesLongitudeLengthBits);

				break;
			}

			// -------------------------------- Autopilot --------------------------------

			// Generic
			case RemoteSystemPacketType::autopilotEnabled: {
				stream.writeBool(rc.getRemoteData().autopilot.autopilot);
				break;
			}

			// Lateral
			case RemoteSystemPacketType::autopilotLateralMode: {
				const auto mode =
					rc.getAircraftData().raw.autopilot.lateralMode == rc.getSettings().autopilot.lateralMode
					? AutopilotLateralMode::dir
					: rc.getSettings().autopilot.lateralMode;

				stream.writeUint8(std::to_underlying(mode), RemoteSystemPacket::autopilotLateralModeLengthBits);

				break;
			}

			case RemoteSystemPacketType::autopilotHeading: {
				stream.writeUint16(rc.getSettings().autopilot.headingDeg, RemoteSystemPacket::autopilotHeadingLengthBits);
				break;
			}
			case RemoteSystemPacketType::autopilotMaxRollAngleRad: {
				stream.writeFloat(rc.getSettings().autopilot.maxRollAngleRad);
				break;
			}
			case RemoteSystemPacketType::autopilotStabilizedModeRollAngleIncrementRadPerSecond: {
				stream.writeFloat(rc.getSettings().autopilot.stabilizedModeRollAngleIncrementRadPerSecond);
				break;
			}
			case RemoteSystemPacketType::autopilotRollAngleLPFFactorPerSecond: {
				stream.writeFloat(rc.getSettings().autopilot.rollAngleLPFFactorPerSecond);
				break;
			}
			case RemoteSystemPacketType::autopilotYawToRollPID: {
				writePID(stream, rc.getSettings().autopilot.PIDs.yawToRoll);
				break;
			}
			case RemoteSystemPacketType::autopilotRollToAileronsPID: {
				writePID(stream, rc.getSettings().autopilot.PIDs.rollToAilerons);
				break;
			}
			case RemoteSystemPacketType::autopilotMaxAileronsPercent: {
				stream.writeUint8(rc.getSettings().autopilot.maxAileronsPercent, RemoteSystemPacket::autopilotPercentLengthBits);
				break;
			}

			// Vertical
			case RemoteSystemPacketType::autopilotVerticalMode: {
				AutopilotVerticalMode mode;

				if (rc.getSettings().autopilot.verticalMode == AutopilotVerticalMode::flc) {
					mode =
						rc.getAircraftData().raw.autopilot.verticalMode == AutopilotVerticalMode::flc
							|| rc.getAircraftData().raw.autopilot.verticalMode == AutopilotVerticalMode::alts
						? AutopilotVerticalMode::dir
						: AutopilotVerticalMode::flc;
				}
				else {
					mode =
						rc.getAircraftData().raw.autopilot.verticalMode == rc.getSettings().autopilot.verticalMode
						? AutopilotVerticalMode::dir
						: rc.getSettings().autopilot.verticalMode;
				}

				stream.writeUint8(std::to_underlying(mode), RemoteSystemPacket::autopilotVerticalModeLengthBits);

				break;
			}
			case RemoteSystemPacketType::autopilotAltitude: {
				writeAltitude(
					stream,
					Units::convertDistance(rc.getSettings().autopilot.altitudeFt, DistanceUnit::foot, DistanceUnit::meter),
					RemoteSystemPacket::autopilotAltitudeLengthBits,
					RemoteSystemPacket::autopilotAltitudeMinM,
					RemoteSystemPacket::autopilotAltitudeMaxM
				);

				break;
			}
			case RemoteSystemPacketType::autopilotMaxPitchAngleRad: {
				stream.writeFloat(rc.getSettings().autopilot.maxPitchAngleRad);
				break;
			}
			case RemoteSystemPacketType::autopilotStabilizedModePitchAngleIncrementRadPerSecond: {
				stream.writeFloat(rc.getSettings().autopilot.stabilizedModePitchAngleIncrementRadPerSecond);
				break;
			}
			case RemoteSystemPacketType::autopilotPitchAngleLPFFactorPerSecond: {
				stream.writeFloat(rc.getSettings().autopilot.pitchAngleLPFFactorPerSecond);
				break;
			}
			case RemoteSystemPacketType::autopilotSpeedToPitchPID: {
				writePID(stream, rc.getSettings().autopilot.PIDs.speedToPitch);
				break;
			}
			case RemoteSystemPacketType::autopilotAltitudeToPitchPID: {
				writePID(stream, rc.getSettings().autopilot.PIDs.altitudeToPitch);
				break;
			}
			case RemoteSystemPacketType::autopilotPitchToElevatorPID: {
				writePID(stream, rc.getSettings().autopilot.PIDs.pitchToElevator);
				break;
			}
			case RemoteSystemPacketType::autopilotMaxElevatorPercent: {
				stream.writeUint8(rc.getSettings().autopilot.maxElevatorPercent, RemoteSystemPacket::autopilotPercentLengthBits);
				break;
			}

			// Longitudinal
			case RemoteSystemPacketType::autopilotAutothrottleEnabled: {
				stream.writeBool(rc.getRemoteData().autopilot.autothrottle);

				break;
			}
			case RemoteSystemPacketType::autopilotSpeed: {
				const auto speedFactor =
					std::min<float>(
						Units::convertSpeed(rc.getSettings().autopilot.speedKt, SpeedUnit::knot, SpeedUnit::meterPerSecond),
						RemoteSystemPacket::autopilotSpeedMaxMPS
					)
					/ static_cast<float>(RemoteSystemPacket::autopilotSpeedMaxMPS);

				const auto speedMapped = static_cast<float>((1 << RemoteSystemPacket::autopilotSpeedLengthBits) - 1) * speedFactor;

				stream.writeUint8(static_cast<uint8_t>(std::round(speedMapped)), RemoteSystemPacket::autopilotSpeedLengthBits);

				break;
			}
			case RemoteSystemPacketType::autopilotSpeedToThrottlePID: {
				writePID(stream, rc.getSettings().autopilot.PIDs.speedToThrottle);
				break;
			}
			case RemoteSystemPacketType::autopilotMinThrottlePercent: {
				stream.writeUint8(rc.getSettings().autopilot.minThrottlePercent, RemoteSystemPacket::autopilotPercentLengthBits);
				break;
			}
			case RemoteSystemPacketType::autopilotMaxThrottlePercent: {
				stream.writeUint8(rc.getSettings().autopilot.maxThrottlePercent, RemoteSystemPacket::autopilotPercentLengthBits);
				break;
			}

			default:
				ESP_LOGE(_logTag, "failed to transmit packet: unsupported type %d", getEnqueuedSystemPacketType());
				break;
		}
	}

	void RemoteTransceiver::writePID(BitStream& stream, const PIDCoefficients& coefficients) {
		stream.writeFloat(coefficients.p);
		stream.writeFloat(coefficients.i);
		stream.writeFloat(coefficients.d);
	}
}

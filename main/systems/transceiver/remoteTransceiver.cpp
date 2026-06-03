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

	void RemoteTransceiver::enqueueAutopilot(const RemoteSystemAutopilotPacketType type) {
		_enqueuedAutopilotPacketType = type;

		enqueueSystemPacket(RemoteSystemPacketType::autopilot);
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
						setCommunicationSettings(config::XCVR::communicationSettings);
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
			case AircraftPacketType::STierTelemetry:
				return receiveAircraftSTierTelemetryPacket(stream, payloadLength);
			
			case AircraftPacketType::ATierTelemetry:
				return receiveAircraftATierTelemetryPacket(stream, payloadLength);

			case AircraftPacketType::BTierTelemetry:
				return receiveAircraftBTierTelemetryPacket(stream, payloadLength);
				
			case AircraftPacketType::system:
				return receiveAircraftSystemPacket(stream, payloadLength);
				
			default:
				ESP_LOGE(_logTag, "failed to receive packet: unsupported type %d", packetType);
				
				return false;
		}
	}
	
	bool RemoteTransceiver::receiveAircraftSTierTelemetryPacket(BitStream& stream, const uint8_t payloadLength) {
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

	bool RemoteTransceiver::receiveAircraftATierTelemetryPacket(BitStream& stream, const uint8_t payloadLength) {
		if (!validatePayloadChecksumAndLength(
			stream,
				// Throttle
				AircraftATierTelemetryPacket::throttleLengthBits
				// Latitude & longitude
				+ AircraftATierTelemetryPacket::latLengthBits
				+ AircraftATierTelemetryPacket::lonLengthBits,
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
		
		// Latitude
		// 25 bits per [-90; 90] deg
		// [0.0; 1.0]
		const auto latFactor =
			static_cast<float>(stream.readUint32(AircraftATierTelemetryPacket::latLengthBits))
			/ static_cast<float>((1 << AircraftATierTelemetryPacket::latLengthBits) - 1);
		
		// [-pi / 2; pi / 2]
		const auto lat = latFactor * std::numbers::pi_v<float> - std::numbers::pi_v<float> / 2.f;
		rc.getAircraftData().raw.coordinates.setLatitude(lat);
		
		// Longitude
		// 26 bits per [0; 360] deg
		// [0.0; 1.0]
		const auto lonFactor =
			static_cast<float>(stream.readUint32(AircraftATierTelemetryPacket::lonLengthBits))
			/ static_cast<float>((1 << AircraftATierTelemetryPacket::lonLengthBits) - 1);
		
		// [-pi; pi]
		const auto lon = lonFactor * std::numbers::pi_v<float> * 2;
		rc.getAircraftData().raw.coordinates.setLongitude(lon);

		return true;
	}

	bool RemoteTransceiver::receiveAircraftBTierTelemetryPacket(BitStream& stream, const uint8_t payloadLength) {
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

	bool RemoteTransceiver::receiveAircraftSystemPacket(BitStream& stream, const uint8_t payloadLength) {
		const auto type = static_cast<AircraftSystemPacketType>(stream.readUint8(AircraftSystemPacket::typeLengthBits));

		switch (type) {
			case AircraftSystemPacketType::calibration:
				receiveAircraftSystemCalibrationPacket(stream, payloadLength);
				break;

			case AircraftSystemPacketType::communicationSettingsACK:
				receiveAircraftSystemCommunicationSettingsACKPacket(stream, payloadLength);
				break;

			default:
				break;
		}

		return true;
	}
	
	bool RemoteTransceiver::receiveAircraftSystemCalibrationPacket(BitStream& stream, const uint8_t payloadLength) {
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

	bool RemoteTransceiver::receiveAircraftSystemCommunicationSettingsACKPacket(BitStream& stream, const uint8_t payloadLength) {
		if (!validatePayloadChecksumAndLength(
			stream,
			AircraftSystemPacket::typeLengthBits,
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

			case RemotePacketType::system:
				transmitRemoteSystemPacket(stream);
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

	void RemoteTransceiver::transmitRemoteSystemPacket(BitStream& stream) {
		// Type
		stream.writeUint8(std::to_underlying(getEnqueuedSystemPacketType()), RemoteSystemPacket::typeLengthBits);

		switch (getEnqueuedSystemPacketType()) {
			case RemoteSystemPacketType::trim:
				transmitRemoteSystemTrimPacket(stream);
				break;

			case RemoteSystemPacketType::lights:
				transmitRemoteSystemLightsPacket(stream);
				break;

			case RemoteSystemPacketType::baro:
				transmitRemoteSystemBaroPacket(stream);
				break;

			case RemoteSystemPacketType::autopilot:
				transmitRemoteSystemAutopilotPacket(stream);
				break;

			case RemoteSystemPacketType::camera:
				transmitRemoteSystemCameraPacket(stream);
				break;

			case RemoteSystemPacketType::motors:
				transmitRemoteSystemMotorsPacket(stream);
				break;

			case RemoteSystemPacketType::calibrate:
				transmitRemoteSystemCalibratePacket(stream);
				break;

			case RemoteSystemPacketType::ADIRS:
				transmitRemoteSystemADIRSPacket(stream);
				break;

			case RemoteSystemPacketType::XCVR:
				transmitRemoteSystemXCVRPacket(stream);
				break;

			default:
				ESP_LOGE(_logTag, "failed to transmit packet: unsupported type %d", getEnqueuedSystemPacketType());
				break;
		}
	}
	
	void RemoteTransceiver::transmitRemoteSystemTrimPacket(BitStream& stream) {
		auto& rc = RC::getInstance();
		
		const auto write = [&stream](const int8_t settingsValue) {
			stream.writeUint16(
				// Mapping from [-100; 100] to [0; bits]
				static_cast<uint16_t>(
					(static_cast<int32_t>(settingsValue) + 100)
					* ((1 << RemoteSystemTrimPacket::valueLengthBits) - 1)
					/ 200
				),
				RemoteSystemTrimPacket::valueLengthBits
			);
		};
		
		write(rc.getSettings().controls.aileronsTrim);
		write(rc.getSettings().controls.elevatorTrim);
		write(rc.getSettings().controls.rudderTrim);
	}
	
	void RemoteTransceiver::transmitRemoteSystemLightsPacket(BitStream& stream) {
		auto& rc = RC::getInstance();
		
		stream.writeBool(rc.getRemoteData().lights.navigation);
		stream.writeBool(rc.getRemoteData().lights.strobe);
		stream.writeBool(rc.getRemoteData().lights.landing);
		stream.writeBool(rc.getRemoteData().lights.cabin);
	}
	
	void RemoteTransceiver::transmitRemoteSystemBaroPacket(BitStream& stream) {
		auto& rc = RC::getInstance();

		// Reference pressure
		stream.writeUint16(
			rc.getSettings().ADIRS.referencePressureSTD ? 10132 : rc.getSettings().ADIRS.referencePressurePa / 10,
			RemoteSystemBaroPacket::referencePressureLengthBits
		);
	}
	
	void RemoteTransceiver::transmitRemoteSystemAutopilotPacket(BitStream& stream) {
		auto& rc = RC::getInstance();

		// Type
		stream.writeUint8(std::to_underlying(_enqueuedAutopilotPacketType), RemoteSystemAutopilotPacket::typeLengthBits);

		// Data
		const auto writePID = [&stream](const PIDCoefficients& coefficients) {
			stream.writeFloat(coefficients.p);
			stream.writeFloat(coefficients.i);
			stream.writeFloat(coefficients.d);
		};

		switch (_enqueuedAutopilotPacketType) {
			// Generic
			case RemoteSystemAutopilotPacketType::setAutopilotEngaged: {
				stream.writeBool(rc.getRemoteData().autopilot.autopilot);
				break;
			}

			// Lateral
			case RemoteSystemAutopilotPacketType::setLateralMode: {
				const auto mode =
					rc.getAircraftData().raw.autopilot.lateralMode == rc.getSettings().autopilot.lateralMode
					? AutopilotLateralMode::dir
					: rc.getSettings().autopilot.lateralMode;

				stream.writeUint8(std::to_underlying(mode), RemoteSystemAutopilotPacket::lateralModeLengthBits);

				break;
			}

			case RemoteSystemAutopilotPacketType::setHeading: {
				stream.writeUint16(rc.getSettings().autopilot.headingDeg, RemoteSystemAutopilotPacket::headingLengthBits);
				break;
			}
			case RemoteSystemAutopilotPacketType::setMaxRollAngleRad: {
				stream.writeFloat(rc.getSettings().autopilot.maxRollAngleRad);
				break;
			}
			case RemoteSystemAutopilotPacketType::setStabilizedModeRollAngleIncrementRadPerSecond: {
				stream.writeFloat(rc.getSettings().autopilot.stabilizedModeRollAngleIncrementRadPerSecond);
				break;
			}
			case RemoteSystemAutopilotPacketType::setRollAngleLPFFactorPerSecond: {
				stream.writeFloat(rc.getSettings().autopilot.rollAngleLPFFactorPerSecond);
				break;
			}
			case RemoteSystemAutopilotPacketType::setYawToRollPID: {
				writePID(rc.getSettings().autopilot.PIDs.yawToRoll);
				break;
			}
			case RemoteSystemAutopilotPacketType::setRollToAileronsPID: {
				writePID(rc.getSettings().autopilot.PIDs.rollToAilerons);
				break;
			}
			case RemoteSystemAutopilotPacketType::setMaxAileronsPercent: {
				stream.writeUint8(rc.getSettings().autopilot.maxAileronsPercent, RemoteSystemAutopilotPacket::percentLengthBits);
				break;
			}

			// Vertical
			case RemoteSystemAutopilotPacketType::setVerticalMode: {
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

				stream.writeUint8(std::to_underlying(mode), RemoteSystemAutopilotPacket::verticalModeLengthBits);

				break;
			}
			case RemoteSystemAutopilotPacketType::setAltitude: {
				writeAltitude(
					stream,
					Units::convertDistance(rc.getSettings().autopilot.altitudeFt, DistanceUnit::foot, DistanceUnit::meter),
					RemoteSystemAutopilotPacket::altitudeLengthBits,
					RemoteSystemAutopilotPacket::altitudeMinM,
					RemoteSystemAutopilotPacket::altitudeMaxM
				);

				break;
			}
			case RemoteSystemAutopilotPacketType::setMaxPitchAngleRad: {
				stream.writeFloat(rc.getSettings().autopilot.maxPitchAngleRad);
				break;
			}
			case RemoteSystemAutopilotPacketType::setStabilizedModePitchAngleIncrementRadPerSecond: {
				stream.writeFloat(rc.getSettings().autopilot.stabilizedModePitchAngleIncrementRadPerSecond);
				break;
			}
			case RemoteSystemAutopilotPacketType::setPitchAngleLPFFactorPerSecond: {
				stream.writeFloat(rc.getSettings().autopilot.pitchAngleLPFFactorPerSecond);
				break;
			}
			case RemoteSystemAutopilotPacketType::setSpeedToPitchPID: {
				writePID(rc.getSettings().autopilot.PIDs.speedToPitch);
				break;
			}
			case RemoteSystemAutopilotPacketType::setAltitudeToPitchPID: {
				writePID(rc.getSettings().autopilot.PIDs.altitudeToPitch);
				break;
			}
			case RemoteSystemAutopilotPacketType::setPitchToElevatorPID: {
				writePID(rc.getSettings().autopilot.PIDs.pitchToElevator);
				break;
			}
			case RemoteSystemAutopilotPacketType::setMaxElevatorPercent: {
				stream.writeUint8(rc.getSettings().autopilot.maxElevatorPercent, RemoteSystemAutopilotPacket::percentLengthBits);
				break;
			}

			// Longitudinal
			case RemoteSystemAutopilotPacketType::setAutothrottleEnabled: {
				stream.writeBool(rc.getRemoteData().autopilot.autothrottle);

				break;
			}
			case RemoteSystemAutopilotPacketType::setSpeed: {
				const auto speedFactor =
					std::min<float>(
						Units::convertSpeed(rc.getSettings().autopilot.speedKt, SpeedUnit::knot, SpeedUnit::meterPerSecond),
						RemoteSystemAutopilotPacket::speedMaxMPS
					)
					/ static_cast<float>(RemoteSystemAutopilotPacket::speedMaxMPS);

				const auto speedMapped = static_cast<float>((1 << RemoteSystemAutopilotPacket::speedLengthBits) - 1) * speedFactor;

				stream.writeUint8(static_cast<uint8_t>(std::round(speedMapped)), RemoteSystemAutopilotPacket::speedLengthBits);

				break;
			}
			case RemoteSystemAutopilotPacketType::setSpeedToThrottlePID: {
				writePID(rc.getSettings().autopilot.PIDs.speedToThrottle);
				break;
			}
			case RemoteSystemAutopilotPacketType::setMinThrottlePercent: {
				stream.writeUint8(rc.getSettings().autopilot.minThrottlePercent, RemoteSystemAutopilotPacket::percentLengthBits);
				break;
			}
			case RemoteSystemAutopilotPacketType::setMaxThrottlePercent: {
				stream.writeUint8(rc.getSettings().autopilot.maxThrottlePercent, RemoteSystemAutopilotPacket::percentLengthBits);
				break;
			}
		}
	}

	void RemoteTransceiver::transmitRemoteSystemCameraPacket(BitStream& stream) {
		auto& rc = RC::getInstance();

		stream.writeInt16(rc.getSettings().controls.cameraPitchDeg, RemoteSystemCameraPacket::pitchLengthBits);
		stream.writeInt16(rc.getSettings().controls.cameraYawDeg, RemoteSystemCameraPacket::yawLengthBits);
	}
	
	void RemoteTransceiver::transmitRemoteSystemMotorsPacket(BitStream& stream) {
		const auto& motor = RC::getInstance().getRemoteData().motorSettings;

		stream.writeUint8(static_cast<uint8_t>(motor.type), RemoteSystemMotorConfigurationPacket::typeLengthBits);
		stream.writeUint16(motor.settings.min, RemoteSystemMotorConfigurationPacket::minLengthBits);
		stream.writeUint16(motor.settings.max, RemoteSystemMotorConfigurationPacket::maxLengthBits);
		stream.writeBool(motor.settings.reverse);
	}
	
	void RemoteTransceiver::transmitRemoteSystemCalibratePacket(BitStream& stream) {
		auto& rc = RC::getInstance();
		
		stream.writeUint8(std::to_underlying(rc.getRemoteData().calibrationSystem), RemoteSystemCalibratePacket::systemLengthBits);

//		ESP_LOGI(_logTag, "Sending calibrate packet");
	}

	void RemoteTransceiver::transmitRemoteSystemADIRSPacket(BitStream& stream) {
		auto& rc = RC::getInstance();

		stream.writeInt16(rc.getSettings().ADIRS.magneticDeclinationDeg, RemoteSystemADIRSPacket::magneticDeclinationLengthBits);
	}

	void RemoteTransceiver::transmitRemoteSystemXCVRPacket(BitStream& stream) {
		const auto& settings = RC::getInstance().getRemoteData().transceiver.communicationSettings;

		ESP_LOGI(_logTag, "transmitting communication settings");

		stream.writeUint16(settings.frequencyHz / 1'000'000, RemoteSystemXCVRPacket::RFFrequencyLengthBits);
		stream.writeUint8(std::to_underlying(settings.bandwidth), RemoteSystemXCVRPacket::bandwidthLengthBits);
		stream.writeUint8(settings.spreadingFactor, RemoteSystemXCVRPacket::spreadingFactorLengthBits);
		stream.writeUint8(std::to_underlying(settings.codingRate), RemoteSystemXCVRPacket::codingRateLengthBits);
		stream.writeUint8(settings.syncWord, RemoteSystemXCVRPacket::syncWordLengthBits);
		stream.writeUint16(settings.preambleLength, RemoteSystemXCVRPacket::preambleLengthLengthBits);

		stream.writeInt8(settings.currentLimitMA, RemoteSystemXCVRPacket::currentLimitMALengthBits);
		stream.writeInt8(settings.powerDBm, RemoteSystemXCVRPacket::powerDBmLengthBits);
	}
}
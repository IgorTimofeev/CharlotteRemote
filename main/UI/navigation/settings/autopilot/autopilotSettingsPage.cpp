#include "autopilotSettingsPage.h"

#include "rc.h"
#include "UI/theme.h"
#include "types/generic.h"
#include "utilities/string.h"

namespace pizda {
	AutopilotSettingsPage::AutopilotSettingsPage() {
		auto& rc = RC::getInstance();

		// ----------------------------- Lateral -----------------------------

		title.setText("Lateral");

		// Angle max
		setupRadTextField(
			_latMaxRoll,
			&rc.getSettings().autopilot.maxRollAngleRad,
			30.f,
			RemoteSystemAutopilotPacketType::setMaxRollAngleRad
		);

		rows += &_latMaxRollTitle;

		addRow(_latRow0);

		// Angle increment
		setupRadTextField(
			_latSMTAIRPS,
			&rc.getSettings().autopilot.stabilizedModeRollAngleIncrementRadPerSecond,
			5,
			RemoteSystemAutopilotPacketType::setStabilizedModeRollAngleIncrementRadPerSecond
		);

		_latRow0 += &_latSMTAIFRSTitle;

		// Angle LPFF
		setupFloatTextField(
			_latTALPFFPS,
			&rc.getSettings().autopilot.rollAngleLPFFactorPerSecond,
			0.6f,
			0.0f,
			1000.0f,
			RemoteSystemAutopilotPacketType::setRollAngleLPFFactorPerSecond
		);

		_latRow0 += &_latTALPFFPSTitle;

		// Surface factor
		setupUint8PercentTextField(
			_latMaxAileronsFactor,
			&rc.getSettings().autopilot.maxAileronsPercent,
			100,
			RemoteSystemAutopilotPacketType::setMaxAileronsPercent
		);

		rows += &_latMaxAileronsFactorTitle;

		// PIDs
		addPID(
			_latYawToRollPIDTitle,
			_latYawToRollPID,
			RemoteSystemAutopilotPacketType::setYawToRollPID,
			&rc.getSettings().autopilot.PIDs.yawToRoll
		);

		addPID(
			_latRollToAileronsPIDTitle,
			_latRollToAileronsPID,
			RemoteSystemAutopilotPacketType::setRollToAileronsPID,
			&rc.getSettings().autopilot.PIDs.rollToAilerons
		);


		// ----------------------------- Vertical -----------------------------

		rows += &_verSeparator;

		Theme::applyPageTitle(&_verTitle);
		rows += &_verTitle;

		// Angle max
		setupRadTextField(
			_verMaxPitch,
			&rc.getSettings().autopilot.maxPitchAngleRad,
			15.f,
			RemoteSystemAutopilotPacketType::setMaxPitchAngleRad
		);

		rows += &_verMaxPitchTitle;

		addRow(_verRow0);

		// Angle increment
		setupRadTextField(
			_verSMTARFPS,
			&rc.getSettings().autopilot.stabilizedModePitchAngleIncrementRadPerSecond,
			5,
			RemoteSystemAutopilotPacketType::setStabilizedModePitchAngleIncrementRadPerSecond
		);

		_verRow0 += &_verSMTAIRPSTitle;

		// Angle LPFF
		setupFloatTextField(
			_verTALPFFPS,
			&rc.getSettings().autopilot.pitchAngleLPFFactorPerSecond,
			0.6f,
			0.0f,
			1000.0f,
			RemoteSystemAutopilotPacketType::setPitchAngleLPFFactorPerSecond
		);

		_verRow0 += &_verTALPFFPSTitle;

		// Surface factor
		setupUint8PercentTextField(
			_verMaxElevatorFactor,
			&rc.getSettings().autopilot.maxElevatorPercent,
			100,
			RemoteSystemAutopilotPacketType::setMaxElevatorPercent
		);

		rows += &_verMaxElevatorFactorTitle;

		// PIDs
		addPID(
			_varAltitudeToPitchPIDTitle,
			_verAltitudeToPitchPID,
			RemoteSystemAutopilotPacketType::setAltitudeToPitchPID,
			&rc.getSettings().autopilot.PIDs.altitudeToPitch
		);

		addPID(
			_verSpeedToPitchPIDTitle,
			_verSpeedToPitchPID,
			RemoteSystemAutopilotPacketType::setSpeedToPitchPID,
			&rc.getSettings().autopilot.PIDs.speedToPitch
		);

		addPID(
			_verPitchToElevatorPIDTitle,
			_verPitchToElevatorPID,
			RemoteSystemAutopilotPacketType::setPitchToElevatorPID,
			&rc.getSettings().autopilot.PIDs.pitchToElevator
		);

		// ----------------------------- Longitudinal -----------------------------

		rows += &_lonSeparator;

		Theme::applyPageTitle(&_lonTitle);
		rows += &_lonTitle;

		addRow(_lonRow0);

		// Min
		setupUint8PercentTextField(
			_lonThrottleMin,
			&rc.getSettings().autopilot.minThrottlePercent,
			100,
			RemoteSystemAutopilotPacketType::setMinThrottlePercent
		);

		_lonRow0 += &_lonThrottleMinTitle;

		// Max
		setupUint8PercentTextField(
			_lonThrottleMax,
			&rc.getSettings().autopilot.maxThrottlePercent,
			100,
			RemoteSystemAutopilotPacketType::setMaxThrottlePercent
		);

		_lonRow0 += &_lonThrottleMaxTitle;

		// PIDs
		addPID(
			_lonSpeedToThrottlePIDTitle,
			_lonSpeedToThrottlePID,
			RemoteSystemAutopilotPacketType::setSpeedToThrottlePID,
			&rc.getSettings().autopilot.PIDs.speedToThrottle
		);
	}

	void AutopilotSettingsPage::setupAnyTextField(TextField& textField, const std::string_view& text, const std::function<void()>& onEnter) {
		Theme::apply(&textField);
		textField.setText(text);

		textField.setOnInput([onEnter](const Key key, std::optional<std::string_view>) {
			if (key != Key::enter)
				return;

			onEnter();
		});
	}

	void AutopilotSettingsPage::setupFloatTextField(TextField& textField, float* value, float fallbackValue, float min, float max, RemoteSystemAutopilotPacketType packetType) {
		setupAnyTextField(
			textField,
			StringUtils::toString(*value),
			[&textField, fallbackValue, value, min, max, packetType] {
				*value = std::clamp(StringUtils::tryParseFloatOr(textField.getText(), fallbackValue), min, max);
				RC::getInstance().getSettings().autopilot.scheduleWrite();

				RC::getInstance().getTransceiver().enqueueAutopilot(packetType);
			}
		);

		textField.setKeyboardLayoutOptions(KeyboardLayoutOptions::numeric | KeyboardLayoutOptions::allowDecimal);
	}

	void AutopilotSettingsPage::setupRadTextField(TextField& textField, float* angleRad, float fallbackAngleDeg, RemoteSystemAutopilotPacketType packetType) {
		setupAnyTextField(
			textField,
			StringUtils::toString(YOBA::round(toDegrees(*angleRad), 2)),
			[&textField, fallbackAngleDeg, angleRad, packetType] {
				*angleRad = toRadians(StringUtils::tryParseFloatOr(textField.getText(), fallbackAngleDeg));
				RC::getInstance().getSettings().autopilot.scheduleWrite();

				RC::getInstance().getTransceiver().enqueueAutopilot(packetType);
			}
		);

		textField.setKeyboardLayoutOptions(KeyboardLayoutOptions::numeric | KeyboardLayoutOptions::allowDecimal);
	}

	void AutopilotSettingsPage::setupUint8PercentTextField(TextField& textField, uint8_t* percent, float fallbackPercent, RemoteSystemAutopilotPacketType packetType) {
		setupAnyTextField(
			textField,
			std::to_string(*percent),
			[&textField, fallbackPercent, percent, packetType] {
				*percent = static_cast<uint8_t>(std::clamp<int32_t>(StringUtils::tryParseInt32Or(textField.getText(), fallbackPercent), 0, 100));
				RC::getInstance().getSettings().autopilot.scheduleWrite();

				RC::getInstance().getTransceiver().enqueueAutopilot(packetType);
			}
		);

		textField.setKeyboardLayoutOptions(KeyboardLayoutOptions::numeric);
	}

	void AutopilotSettingsPage::addRow(RelativeStackLayout& row) {
		row.setOrientation(Orientation::horizontal);
		row.setGap(Theme::horizontalGap);
		rows += &row;
	}

	void AutopilotSettingsPage::addPID(Titler& titler, PIDReferencer& referencer,
		RemoteSystemAutopilotPacketType packetType, PIDCoefficients* settingsCoefficients) {
		referencer.setCoefficients(*settingsCoefficients);

		referencer.setOnCoefficientsChanged([settingsCoefficients, packetType](const PIDCoefficients& newCoefficients) {
			auto& rc = RC::getInstance();

			*settingsCoefficients = newCoefficients;
			rc.getSettings().autopilot.scheduleWrite();

			rc.getTransceiver().enqueueAutopilot(packetType);
		});

		rows += &titler;
	}
}

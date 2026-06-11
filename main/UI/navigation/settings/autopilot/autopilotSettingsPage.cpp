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
			RemoteSystemPacketType::autopilotMaxRollAngleRad
		);

		rows += &_latMaxRollTitle;

		addRow(_latRow0);

		// Angle increment
		setupRadTextField(
			_latSMTAIRPS,
			&rc.getSettings().autopilot.stabilizedModeRollAngleIncrementRadPerSecond,
			5,
			RemoteSystemPacketType::autopilotStabilizedModeRollAngleIncrementRadPerSecond
		);

		_latRow0 += &_latSMTAIFRSTitle;

		// Angle LPFF
		setupFloatTextField(
			_latTALPFFPS,
			&rc.getSettings().autopilot.rollAngleLPFFactorPerSecond,
			0.6f,
			0.0f,
			1000.0f,
			RemoteSystemPacketType::autopilotRollAngleLPFFactorPerSecond
		);

		_latRow0 += &_latTALPFFPSTitle;

		// Surface factor
		setupUint8PercentTextField(
			_latMaxAileronsFactor,
			&rc.getSettings().autopilot.maxAileronsPercent,
			100,
			RemoteSystemPacketType::autopilotMaxAileronsPercent
		);

		rows += &_latMaxAileronsFactorTitle;

		// PIDs
		addPID(
			_latYawToRollPIDTitle,
			_latYawToRollPID,
			RemoteSystemPacketType::autopilotYawToRollPID,
			&rc.getSettings().autopilot.PIDs.yawToRoll
		);

		addPID(
			_latRollToAileronsPIDTitle,
			_latRollToAileronsPID,
			RemoteSystemPacketType::autopilotRollToAileronsPID,
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
			RemoteSystemPacketType::autopilotMaxPitchAngleRad
		);

		rows += &_verMaxPitchTitle;

		addRow(_verRow0);

		// Angle increment
		setupRadTextField(
			_verSMTARFPS,
			&rc.getSettings().autopilot.stabilizedModePitchAngleIncrementRadPerSecond,
			5,
			RemoteSystemPacketType::autopilotStabilizedModePitchAngleIncrementRadPerSecond
		);

		_verRow0 += &_verSMTAIRPSTitle;

		// Angle LPFF
		setupFloatTextField(
			_verTALPFFPS,
			&rc.getSettings().autopilot.pitchAngleLPFFactorPerSecond,
			0.6f,
			0.0f,
			1000.0f,
			RemoteSystemPacketType::autopilotPitchAngleLPFFactorPerSecond
		);

		_verRow0 += &_verTALPFFPSTitle;

		// Surface factor
		setupUint8PercentTextField(
			_verMaxElevatorFactor,
			&rc.getSettings().autopilot.maxElevatorPercent,
			100,
			RemoteSystemPacketType::autopilotMaxElevatorPercent
		);

		rows += &_verMaxElevatorFactorTitle;

		// PIDs
		addPID(
			_varAltitudeToPitchPIDTitle,
			_verAltitudeToPitchPID,
			RemoteSystemPacketType::autopilotAltitudeToPitchPID,
			&rc.getSettings().autopilot.PIDs.altitudeToPitch
		);

		addPID(
			_verSpeedToPitchPIDTitle,
			_verSpeedToPitchPID,
			RemoteSystemPacketType::autopilotSpeedToPitchPID,
			&rc.getSettings().autopilot.PIDs.speedToPitch
		);

		addPID(
			_verPitchToElevatorPIDTitle,
			_verPitchToElevatorPID,
			RemoteSystemPacketType::autopilotPitchToElevatorPID,
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
			RemoteSystemPacketType::autopilotMinThrottlePercent
		);

		_lonRow0 += &_lonThrottleMinTitle;

		// Max
		setupUint8PercentTextField(
			_lonThrottleMax,
			&rc.getSettings().autopilot.maxThrottlePercent,
			100,
			RemoteSystemPacketType::autopilotMaxThrottlePercent
		);

		_lonRow0 += &_lonThrottleMaxTitle;

		// PIDs
		addPID(
			_lonSpeedToThrottlePIDTitle,
			_lonSpeedToThrottlePID,
			RemoteSystemPacketType::autopilotSpeedToThrottlePID,
			&rc.getSettings().autopilot.PIDs.speedToThrottle
		);

		// Initialization
		scrollView.setVerticalPosition(_scrollPosition);
	}

	AutopilotSettingsPage::~AutopilotSettingsPage() {
		_scrollPosition = scrollView.getVerticalPosition();
	}

	int32_t AutopilotSettingsPage::_scrollPosition = 0;

	void AutopilotSettingsPage::setupAnyTextField(TextField& textField, const std::string_view& text, const std::function<void()>& onEnter) {
		Theme::apply(&textField);
		textField.setText(text);

		textField.setOnInput([onEnter](const Key key, std::optional<std::string_view>) {
			if (key != Key::enter)
				return;

			onEnter();
		});
	}

	void AutopilotSettingsPage::setupFloatTextField(TextField& textField, float* value, float fallbackValue, float min, float max, RemoteSystemPacketType packetType) {
		setupAnyTextField(
			textField,
			std::to_string(*value),
			[&textField, fallbackValue, value, min, max, packetType] {
				auto& rc = RC::getInstance();

				*value = std::clamp(StringUtils::tryParseFloatOr(textField.getText(), fallbackValue), min, max);
				rc.getSettings().autopilot.writeLater();

				rc.getTransceiver().enqueueSystemPacket(packetType);
			}
		);

		textField.setKeyboardLayoutOptions(KeyboardLayoutOptions::numeric | KeyboardLayoutOptions::allowDecimal);
	}

	void AutopilotSettingsPage::setupRadTextField(TextField& textField, float* angleRad, float fallbackAngleDeg, RemoteSystemPacketType packetType) {
		setupAnyTextField(
			textField,
			std::to_string(YOBA::round(toDegrees(*angleRad), 2)),
			[&textField, fallbackAngleDeg, angleRad, packetType] {
				auto& rc = RC::getInstance();

				*angleRad = toRadians(StringUtils::tryParseFloatOr(textField.getText(), fallbackAngleDeg));
				rc.getSettings().autopilot.writeLater();

				rc.getTransceiver().enqueueSystemPacket(packetType);
			}
		);

		textField.setKeyboardLayoutOptions(KeyboardLayoutOptions::numeric | KeyboardLayoutOptions::allowDecimal);
	}

	void AutopilotSettingsPage::setupUint8PercentTextField(TextField& textField, uint8_t* percent, float fallbackPercent, RemoteSystemPacketType packetType) {
		setupAnyTextField(
			textField,
			std::to_string(*percent),
			[&textField, fallbackPercent, percent, packetType] {
				auto& rc = RC::getInstance();

				*percent = static_cast<uint8_t>(std::clamp<int32_t>(StringUtils::tryParseInt32Or(textField.getText(), fallbackPercent), 0, 100));
				rc.getSettings().autopilot.writeLater();

				rc.getTransceiver().enqueueSystemPacket(packetType);
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
		RemoteSystemPacketType packetType, PIDCoefficients* settingsCoefficients) {
		referencer.setCoefficients(*settingsCoefficients);

		referencer.setOnCoefficientsChanged([settingsCoefficients, packetType](const PIDCoefficients& newCoefficients) {
			auto& rc = RC::getInstance();

			*settingsCoefficients = newCoefficients;
			rc.getSettings().autopilot.writeLater();

			rc.getTransceiver().enqueueSystemPacket(packetType);
		});

		rows += &titler;
	}
}

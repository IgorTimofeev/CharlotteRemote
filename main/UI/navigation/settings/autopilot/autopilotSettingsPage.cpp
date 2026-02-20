#include "autopilotSettingsPage.h"

#include "rc.h"
#include "UI/theme.h"
#include "types/generic.h"
#include "utilities/string.h"

namespace pizda {
	AutopilotSettingsPage::AutopilotSettingsPage() {
		auto& rc = RC::getInstance();

		// ----------------------------- Limits -----------------------------

		title.setText(L"Limits");

		_anglesRow.setOrientation(Orientation::horizontal);
		_anglesRow.setGap(Theme::horizontalGap);
		rows += &_anglesRow;

		// Max roll
		setupRadTextField(
			_anglesMaxRoll,
			&rc.getSettings().autopilot.maxRollAngleRad,
			30.f,
			RemoteAuxiliaryAutopilotPacketType::setMaxRollAngleRad
		);

		_anglesRow += &_anglesMaxRollTitle;

		// Max pitch
		setupRadTextField(
			_anglesMaxPitch,
			&rc.getSettings().autopilot.maxPitchAngleRad,
			15.f,
			RemoteAuxiliaryAutopilotPacketType::setMaxPitchAngleRad
		);

		_anglesRow += &_anglesMaxPitchTitle;

		// ----------------------------- LPFF -----------------------------

		rows += &_targetAnglesSeparator;

		Theme::applyPageTitle(&_targetAnglesTitle);
		rows += &_targetAnglesTitle;

		_targetAnglesRow.setOrientation(Orientation::horizontal);
		_targetAnglesRow.setGap(Theme::horizontalGap);
		rows += &_targetAnglesRow;

		// Default
		setupFloatTextField(
			_targetAngleLPFF,
			&rc.getSettings().autopilot.targetAngleLPFF,
			0.6f,
			0.0f,
			100.0f,
			RemoteAuxiliaryAutopilotPacketType::setTargetAngleLPFF
		);

		_targetAnglesRow += &_targetAngleLPFFTitle;

		// Stab
		setupFloatTextField(
			_stabAngleIncrement,
			&rc.getSettings().autopilot.stabTargetAngleIncrementFactor,
			1.0f,
			0.0f,
			1000.0f,
			RemoteAuxiliaryAutopilotPacketType::setStabAngleIncrementRad
		);

		_targetAnglesRow += &_stabAngleIncrementTitle;

		// ----------------------------- Controls -----------------------------

		rows += &_surfacesSeparator;

		Theme::applyPageTitle(&_surfacesTitle);
		rows += &_surfacesTitle;

		_surfacesRow.setOrientation(Orientation::horizontal);
		_surfacesRow.setGap(Theme::horizontalGap);
		rows += &_surfacesRow;

		// Ailerons
		setupFloatTextField(
			_maxAileronsFactor,
			&rc.getSettings().autopilot.maxAileronsFactor,
			1.0f,
			0.0f,
			1.0f,
			RemoteAuxiliaryAutopilotPacketType::setMaxAileronsFactor
		);

		_surfacesRow += &_maxAileronsFactorTitle;

		// Elevator
		setupFloatTextField(
			_maxElevatorFactor,
			&rc.getSettings().autopilot.maxElevatorFactor,
			1.0f,
			0.0f,
			1.0f,
			RemoteAuxiliaryAutopilotPacketType::setMaxElevatorFactor
		);

		_surfacesRow += &_maxElevatorFactorTitle;

		// ----------------------------- PIDs -----------------------------

		rows += &_PIDSeparator;

		Theme::applyPageTitle(&_PIDTitle);
		rows += &_PIDTitle;

		const auto addPID = [this, &rc](Titler& titler, PIDReferencer& referencer, RemoteAuxiliaryAutopilotPacketType packetType, PIDCoefficients* settingsCoefficients) {
			referencer.setCoefficients(*settingsCoefficients);

			referencer.setOnCoefficientsChanged([&rc, settingsCoefficients, packetType](const PIDCoefficients& newCoefficients) {
				*settingsCoefficients = newCoefficients;
				rc.getSettings().autopilot.scheduleWrite();

				rc.getTransceiver().enqueueAutopilot(packetType);
			});

			rows += &titler;
		};

		addPID(
			_yawToRollPIDTitle,
			_yawToRollPID,
			RemoteAuxiliaryAutopilotPacketType::setYawToRollPID,
			&rc.getSettings().autopilot.PIDs.yawToRoll
		);

		addPID(
			_altitudeToPitchPIDTitle,
			_altitudeToPitchPID,
			RemoteAuxiliaryAutopilotPacketType::setAltitudeToPitchPID,
			&rc.getSettings().autopilot.PIDs.altitudeToPitch
		);

		addPID(
			_speedToPitchPIDTitle,
			_speedToPitchPID,
			RemoteAuxiliaryAutopilotPacketType::setSpeedToPitchPID,
			&rc.getSettings().autopilot.PIDs.speedToPitch
		);

		addPID(
			_rollToAileronsPIDTitle,
			_rollToAileronsPID,
			RemoteAuxiliaryAutopilotPacketType::setRollToAileronsPID,
			&rc.getSettings().autopilot.PIDs.rollToAilerons
		);

		addPID(
			_pitchToElevatorPIDTitle,
			_pitchToElevatorPID,
			RemoteAuxiliaryAutopilotPacketType::setPitchToElevatorPID,
			&rc.getSettings().autopilot.PIDs.pitchToElevator
		);
	}

	void AutopilotSettingsPage::setupAnyTextField(TextField& textField, const std::wstring_view& text, const std::function<void()>& onEnter) {
		Theme::apply(&textField);
		textField.setText(text);

		textField.setOnInput([onEnter](const Key key, std::optional<std::wstring_view>) {
			if (key != Key::enter)
				return;

			onEnter();
		});
	}

	void AutopilotSettingsPage::setupFloatTextField(TextField& textField, float* value, float fallbackValue, float min, float max, RemoteAuxiliaryAutopilotPacketType packetType) {
		setupAnyTextField(
			textField,
			StringUtils::toWString(*value),
			[&textField, fallbackValue, value, min, max, packetType] {
				*value = std::clamp(StringUtils::tryParseFloatOr(textField.getText(), fallbackValue), min, max);
				RC::getInstance().getSettings().autopilot.scheduleWrite();

				RC::getInstance().getTransceiver().enqueueAutopilot(packetType);
			}
		);

		textField.setKeyboardLayoutOptions(KeyboardLayoutOptions::numeric | KeyboardLayoutOptions::allowDecimal);
	}

	void AutopilotSettingsPage::setupRadTextField(TextField& textField, float* angleRad, float fallbackAngleDeg, RemoteAuxiliaryAutopilotPacketType packetType) {
		setupAnyTextField(
			textField,
			StringUtils::toWString(YOBA::round(toDegrees(*angleRad), 2)),
			[&textField, fallbackAngleDeg, angleRad, packetType] {
				*angleRad = toRadians(StringUtils::tryParseFloatOr(textField.getText(), fallbackAngleDeg));
				RC::getInstance().getSettings().autopilot.scheduleWrite();

				RC::getInstance().getTransceiver().enqueueAutopilot(packetType);
			}
		);

		textField.setKeyboardLayoutOptions(KeyboardLayoutOptions::numeric | KeyboardLayoutOptions::allowDecimal);
	}
}

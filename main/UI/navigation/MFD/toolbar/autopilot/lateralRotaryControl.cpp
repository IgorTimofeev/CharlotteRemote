#include "lateralRotaryControl.h"

#include "rc.h"

namespace pizda {
	LateralRotaryControlStab::LateralRotaryControlStab() {
		setDigitCount(3);
		setSignVisible(true);

		setActiveColor(&Theme::green1);
	}

	void LateralRotaryControlStab::onTick() {
		RotaryControlSevenVariant::onTick();

		setValue(static_cast<int32_t>(std::round(toDegrees(RC::getInstance().getAircraftData().computed.autopilot.rollRad))));
	}

	LateralRotaryControl::LateralRotaryControl() {
		setVariants({
			&seven,
			&stab
		});

		seven.setValue(RC::getInstance().getSettings().autopilot.headingDeg);

		switch (RC::getInstance().getSettings().autopilot.lateralMode) {
			case AutopilotLateralMode::stab:
				setVariantIndex(1);
				break;

			default:
				setVariantIndex(0);
				break;
		}
	}
	
	std::wstring_view LateralRotaryControl::variantIndexToTitle(const uint8_t index) {
		switch (index) {
			case 0: return L"HDG";
			default: return L"LSTB";
		}
	}
	
	bool LateralRotaryControl::isVariantEditable(const uint8_t index) {
		return index == 0;
	}
	
	void LateralRotaryControl::onRotate(const bool clockwise, const bool big) {
		SevenRotaryControl::onRotate(clockwise, big);

		auto& rc = RC::getInstance();

		rc.getSettings().autopilot.headingDeg = static_cast<uint16_t>(seven.getValue());
		rc.getSettings().autopilot.scheduleWrite();

		rc.getTransceiver().enqueueAutopilot(RemoteAuxiliaryAutopilotPacketType::setHeading);
	}

	void LateralRotaryControl::onVariantChanged() {
		auto& rc = RC::getInstance();

		switch (getVariantIndex()) {
			case 0:
				rc.getSettings().autopilot.lateralMode = AutopilotLateralMode::hdg;
				break;

			default:
				rc.getSettings().autopilot.lateralMode = AutopilotLateralMode::stab;
				break;
		}

		rc.getSettings().autopilot.scheduleWrite();
	}
	
	void LateralRotaryControl::onPress() {
		RotaryControl::onPress();

		auto& rc = RC::getInstance();

		rc.getTransceiver().enqueueAutopilot(RemoteAuxiliaryAutopilotPacketType::setLateralMode);
	}
	
	void LateralRotaryControl::onTick() {
		RotaryControl::onTick();
		
		switch (RC::getInstance().getAircraftData().raw.autopilot.lateralMode) {
			case AutopilotLateralMode::dir:
				setBorderColor(nullptr);
				break;

			case AutopilotLateralMode::stab:
				setBorderColor(getVariantIndex() == 1 ? &Theme::fg1 : &Theme::yellow);
				break;

			case AutopilotLateralMode::hdg:
				setBorderColor(getVariantIndex() == 0 ? &Theme::fg1 : &Theme::yellow);
				break;
		}
	}
}
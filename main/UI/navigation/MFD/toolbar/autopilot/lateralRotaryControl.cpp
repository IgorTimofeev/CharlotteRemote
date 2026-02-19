#include "lateralRotaryControl.h"

#include "rc.h"

namespace pizda {
	LateralRotaryControlStab::LateralRotaryControlStab() {
		setDigitCount(3);
		setSignVisible(true);

		setActiveColor(&Theme::magenta1);
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

		switch (RC::getInstance().getRemoteData().autopilot.lateralMode) {
			case AutopilotLateralMode::stab:
				setVariantIndex(1);
				break;

			default:
				setVariantIndex(0);
				break;
		}

		setVariantIndex(RC::getInstance().getRemoteData().autopilot.lateralMode == AutopilotLateralMode::stab ? 1 : 0);
		
		seven.setValue(RC::getInstance().getSettings().autopilot.headingDeg);
	}
	
	std::wstring_view LateralRotaryControl::variantIndexToTitle(const uint8_t index) {
		switch (index) {
			case 0: return L"HDG";
			default: return L"STB";
		}
	}
	
	bool LateralRotaryControl::isVariantEditable(const uint8_t index) {
		return index == 0;
	}
	
	void LateralRotaryControl::onRotate(const bool clockwise, const bool big) {
		SevenRotaryControl::onRotate(clockwise, big);
		
		RC::getInstance().getSettings().autopilot.headingDeg = static_cast<uint16_t>(seven.getValue());
		RC::getInstance().getSettings().autopilot.scheduleWrite();
		
		RC::getInstance().getTransceiver().enqueueAuxiliary(RemoteAuxiliaryPacketType::autopilot);
	}
	
	void LateralRotaryControl::onPress() {
		RotaryControl::onPress();

		AutopilotLateralMode newMode;

		switch (getVariantIndex()) {
			case 0: {
				newMode = AutopilotLateralMode::hdg;
				break;
			}
			default: {
				newMode = AutopilotLateralMode::stab;
				break;
			}
		}
		
		RC::getInstance().getRemoteData().autopilot.lateralMode =
			RC::getInstance().getRemoteData().autopilot.lateralMode == newMode
			? AutopilotLateralMode::dir
			: newMode;
		
		RC::getInstance().getTransceiver().enqueueAuxiliary(RemoteAuxiliaryPacketType::autopilot);
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
#include "lateralRotaryControl.h"

#include "rc.h"

namespace pizda {
	LateralRotaryControl::LateralRotaryControl() {
		setVariants({
			&seven,
			&LNAV
		});
		
		seven.setValue(RC::getInstance().getSettings().autopilot.headingDeg);
	}
	
	std::wstring_view LateralRotaryControl::variantIndexToTitle(uint8_t index) {
		switch (index) {
			case 0: return L"HDG";
			default: return L"LNAV";
		}
	}
	
	bool LateralRotaryControl::isVariantEditable(uint8_t index) {
		return index == 0;
	}
	
	void LateralRotaryControl::onRotate(bool clockwise, bool big) {
		SevenRotaryControl::onRotate(clockwise, big);
		
		RC::getInstance().getSettings().autopilot.headingDeg = static_cast<uint16_t>(seven.getValue());
		RC::getInstance().getSettings().autopilot.scheduleWrite();
		
		RC::getInstance().getPacketHandler().enqueue(RemotePacketType::autopilot);
	}
	
	void LateralRotaryControl::onLongPress() {
		RotaryControl::onLongPress();
		
		const auto newMode =
			getVariantIndex() == 0
			? AutopilotLateralMode::hdg
			: AutopilotLateralMode::lnav;
		
		RC::getInstance().getRemoteData().autopilot.lateralMode =
			RC::getInstance().getRemoteData().autopilot.lateralMode == newMode
			? AutopilotLateralMode::man
			: newMode;
			
		RC::getInstance().getPacketHandler().enqueue(RemotePacketType::autopilot);
	}
	
	void LateralRotaryControl::onTick() {
		RotaryControl::onTick();
		
		switch (RC::getInstance().getAircraftData().raw.autopilot.lateralMode) {
			case AutopilotLateralMode::man:
				setBorderColor(nullptr);
				break;
			
			case AutopilotLateralMode::hdg:
				setBorderColor(getVariantIndex() == 0 ? &Theme::fg1 : &Theme::yellow);
				break;
			
			case AutopilotLateralMode::lnav:
				setBorderColor(getVariantIndex() == 1 ? &Theme::fg1 : &Theme::yellow);
				break;
		}
	}
}
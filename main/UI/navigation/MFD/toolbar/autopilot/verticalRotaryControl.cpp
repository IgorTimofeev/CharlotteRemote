#include "verticalRotaryControl.h"

#include "rc.h"

namespace pizda {
	VerticalRotaryControl::VerticalRotaryControl() {
		setVariants({
			&seven,
			&ALT,
			&VNAV
		});
		
		seven.setValue(RC::getInstance().getSettings().autopilot.altitudeFt);
	}
	
	std::wstring_view VerticalRotaryControl::variantIndexToTitle(uint8_t index) {
		switch (index) {
			case 0: return L"FLC";
			case 1: return L"ALT";
			default: return L"VNAV";
		}
	}
	
	bool VerticalRotaryControl::isVariantEditable(uint8_t index) {
		return index == 0;
	}
	
	void VerticalRotaryControl::onRotate(bool clockwise, bool big) {
		SevenRotaryControl::onRotate(clockwise, big);
		
		RC::getInstance().getSettings().autopilot.altitudeFt = static_cast<uint16_t>(seven.getValue());
		RC::getInstance().getSettings().autopilot.scheduleWrite();
		
		RC::getInstance().getPacketHandler().enqueue(RemotePacketType::autopilot);
	}
	
	void VerticalRotaryControl::onLongPress() {
		RotaryControl::onLongPress();
		
		const auto newMode =
			getVariantIndex() == 0
			? AutopilotVerticalMode::flc
			: (
				getVariantIndex() == 1
				? AutopilotVerticalMode::alt
				: AutopilotVerticalMode::vnav
			);
		
		RC::getInstance().getRemoteData().autopilot.verticalMode =
			RC::getInstance().getRemoteData().autopilot.verticalMode == newMode
			? AutopilotVerticalMode::man
			: newMode;
		
		RC::getInstance().getPacketHandler().enqueue(RemotePacketType::autopilot);
	}
	
	void VerticalRotaryControl::onTick() {
		RotaryControl::onTick();
		
		switch (RC::getInstance().getAircraftData().raw.autopilot.verticalMode) {
			case AutopilotVerticalMode::man:
				setBorderColor(nullptr);
				break;
			
			case AutopilotVerticalMode::flc:
				setBorderColor(getVariantIndex() == 0 ? &Theme::fg1 : &Theme::yellow);
				break;
			
			case AutopilotVerticalMode::alts:
				setBorderColor(getVariantIndex() == 1 ? &Theme::fg1 : &Theme::yellow);
				break;
			
			case AutopilotVerticalMode::alt:
				setBorderColor(getVariantIndex() == 1 ? &Theme::fg1 : &Theme::yellow);
				break;
			
			case AutopilotVerticalMode::vnav:
				setBorderColor(getVariantIndex() == 2 ? &Theme::fg1 : &Theme::yellow);
				break;
		}
	}
}
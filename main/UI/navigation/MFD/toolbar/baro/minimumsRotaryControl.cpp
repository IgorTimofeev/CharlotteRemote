#include "minimumsRotaryControl.h"

#include "rc.h"

namespace pizda {
	MinimumsRotaryControl::MinimumsRotaryControl() {
		setVariants({
			&seven
		});
		
		seven.setValue(RC::getInstance().getSettings().controls.minimumAltitudeFt);
		updateColor();
	}
	
	std::wstring_view MinimumsRotaryControl::variantIndexToTitle(uint8_t index) {
		return L"Minimums";
	}
	
	bool MinimumsRotaryControl::isVariantEditable(uint8_t index) {
		return index == 0;
	}
	
	void MinimumsRotaryControl::onRotate(bool clockwise, bool big) {
		SevenRotaryControl::onRotate(clockwise, big);
		
		RC::getInstance().getSettings().controls.minimumAltitudeFt = static_cast<uint16_t>(seven.getValue());
		RC::getInstance().getSettings().controls.scheduleWrite();
	}
	
	void MinimumsRotaryControl::onPress() {
		RotaryControl::onPress();
		
		RC::getInstance().getSettings().controls.minimumAltitudeEnabled = !RC::getInstance().getSettings().controls.minimumAltitudeEnabled;
		RC::getInstance().getSettings().controls.scheduleWrite();
		
		updateColor();
	}
	
	void MinimumsRotaryControl::onTick() {
		RotaryControl::onTick();
		
		updateColor();
	}
	
	void MinimumsRotaryControl::updateColor() {
		setBorderColor(
			RC::getInstance().getSettings().controls.minimumAltitudeEnabled
			? &Theme::fg1
			: nullptr
		);
	}
}
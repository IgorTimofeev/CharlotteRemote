#include "minimumsRotaryControl.h"

#include "rc.h"

namespace pizda {
	MinimumsRotaryControl::MinimumsRotaryControl() : SevenRotaryControl(4, 0, 15000, false, 1, 10) {
		setVariants({
			&seven
		});

		setVariantIndex(0);

		seven.setValue(RC::getInstance().getSettings().ADIRS.minimumAltitudeFt);
		updateColor();
	}
	
	std::string_view MinimumsRotaryControl::variantIndexToTitle(uint8_t index) {
		return "Minimums";
	}
	
	bool MinimumsRotaryControl::isVariantEditable(const uint8_t index) {
		return index == 0;
	}
	
	void MinimumsRotaryControl::onRotate(const bool clockwise, const bool big) {
		SevenRotaryControl::onRotate(clockwise, big);
		
		RC::getInstance().getSettings().ADIRS.minimumAltitudeFt = static_cast<uint16_t>(seven.getValue());
		RC::getInstance().getSettings().ADIRS.writeLater();
	}
	
	void MinimumsRotaryControl::onPress() {
		RotaryControl::onPress();
		
		RC::getInstance().getSettings().ADIRS.minimumAltitudeEnabled = !RC::getInstance().getSettings().ADIRS.minimumAltitudeEnabled;
		RC::getInstance().getSettings().ADIRS.writeLater();
		
		updateColor();
	}
	
	void MinimumsRotaryControl::onTick() {
		RotaryControl::onTick();
		
		updateColor();
	}
	
	void MinimumsRotaryControl::updateColor() {
		setBorderColor(
			RC::getInstance().getSettings().ADIRS.minimumAltitudeEnabled
			? &Theme::fg1
			: nullptr
		);
	}
}
#include "referencePressureRotaryControl.h"

#include "rc.h"

namespace pizda {
	ReferencePressureRotaryControl::ReferencePressureRotaryControl() {
		setVariants({
			&seven
		});

		setVariantIndex(0);

		seven.setDecimalSeparatorIndex(0);
		seven.setValue(RC::getInstance().getSettings().ADIRS.referencePressurePa / 10);
		
		updateColor();
	}
	
	std::wstring_view ReferencePressureRotaryControl::variantIndexToTitle(uint8_t index) {
		return L"Baro";
	}
	
	bool ReferencePressureRotaryControl::isVariantEditable(const uint8_t index) {
		return index == 0;
	}
	
	void ReferencePressureRotaryControl::onRotate(const bool clockwise, const bool big) {
		SevenRotaryControl::onRotate(clockwise, big);
		
		RC::getInstance().getSettings().ADIRS.referencePressurePa = static_cast<uint16_t>(seven.getValue()) * 10;
		RC::getInstance().getSettings().ADIRS.scheduleWrite();
		
		RC::getInstance().getTransceiver().enqueueAuxiliary(RemoteAuxiliaryPacketType::baro);
	}
	
	void ReferencePressureRotaryControl::onPress() {
		RotaryControl::onPress();
		
		RC::getInstance().getSettings().ADIRS.referencePressureSTD = !RC::getInstance().getSettings().ADIRS.referencePressureSTD;
		RC::getInstance().getSettings().ADIRS.scheduleWrite();
		
		updateColor();
		
		RC::getInstance().getTransceiver().enqueueAuxiliary(RemoteAuxiliaryPacketType::baro);
	}
	
	void ReferencePressureRotaryControl::onTick() {
		RotaryControl::onTick();
		
		updateColor();
	}
	
	void ReferencePressureRotaryControl::updateColor() {
		setBorderColor(RC::getInstance().getSettings().ADIRS.referencePressureSTD ? Theme::fg1 : Theme::bg1);
	}
}
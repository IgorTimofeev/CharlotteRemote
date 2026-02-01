#include "speedRotaryControl.h"

#include "rc.h"

namespace pizda {
	SpeedRotaryControl::SpeedRotaryControl() {
		setVariants({
			&seven
		});
		
		seven.setValue(RC::getInstance().getSettings().autopilot.speedKt);
	}
	
	std::wstring_view SpeedRotaryControl::variantIndexToTitle(uint8_t index) {
		return L"IAS";
	}
	
	bool SpeedRotaryControl::isVariantEditable(uint8_t index) {
		return index == 0;
	}
	
	void SpeedRotaryControl::onRotate(bool clockwise, bool big) {
		SevenRotaryControl::onRotate(clockwise, big);
		
		RC::getInstance().getSettings().autopilot.speedKt = static_cast<uint16_t>(seven.getValue());
		RC::getInstance().getSettings().autopilot.scheduleWrite();
		
		RC::getInstance().getTransceiver().enqueueAuxiliary(RemoteAuxiliaryPacketType::autopilot);
	}
	
	void SpeedRotaryControl::onPress() {
		RotaryControl::onPress();

		RC::getInstance().getRemoteData().autopilot.autothrottle = !RC::getInstance().getRemoteData().autopilot.autothrottle;
		
		RC::getInstance().getTransceiver().enqueueAuxiliary(RemoteAuxiliaryPacketType::autopilot);
	}
	
	void SpeedRotaryControl::onTick() {
		RotaryControl::onTick();
		
		setBorderColor(
			RC::getInstance().getAircraftData().raw.autopilot.autothrottle
			? &Theme::fg1
			: nullptr
		);
	}
}
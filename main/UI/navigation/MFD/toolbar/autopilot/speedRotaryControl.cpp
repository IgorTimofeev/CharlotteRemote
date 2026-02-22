#include "speedRotaryControl.h"

#include "rc.h"

namespace pizda {
	SpeedRotaryControl::SpeedRotaryControl() {
		setVariants({
			&seven
		});

		setVariantIndex(0);

		seven.setValue(RC::getInstance().getSettings().autopilot.speedKt);
	}
	
	std::wstring_view SpeedRotaryControl::variantIndexToTitle(uint8_t index) {
		return L"IAS";
	}
	
	bool SpeedRotaryControl::isVariantEditable(const uint8_t index) {
		return index == 0;
	}
	
	void SpeedRotaryControl::onRotate(const bool clockwise, const bool big) {
		SevenRotaryControl::onRotate(clockwise, big);

		auto& rc = RC::getInstance();

		rc.getSettings().autopilot.speedKt = static_cast<uint16_t>(seven.getValue());
		rc.getSettings().autopilot.scheduleWrite();

		rc.getTransceiver().enqueueAutopilot(RemoteAuxiliaryAutopilotPacketType::setSpeed);
	}
	
	void SpeedRotaryControl::onPress() {
		RotaryControl::onPress();

		auto& rc = RC::getInstance();

		rc.getRemoteData().autopilot.autothrottle = !RC::getInstance().getRemoteData().autopilot.autothrottle;

		rc.getTransceiver().enqueueAutopilot(RemoteAuxiliaryAutopilotPacketType::setAutothrottleEnabled);
	}
	
	void SpeedRotaryControl::onTick() {
		RotaryControl::onTick();
		
		setBorderColor(RC::getInstance().getAircraftData().raw.autopilot.autothrottle ? Theme::fg1 : Theme::bg1);
	}
}
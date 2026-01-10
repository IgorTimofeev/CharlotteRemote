#include "lateralRotaryControl.h"

#include "rc.h"

namespace pizda {
	void LateralRotaryControlLNAV::onRender(Renderer* renderer, const Bounds& bounds) {
		auto text = L"ULLI";
		
		renderer->renderString(
			Point(
				bounds.getXCenter() - Theme::fontNormal.getWidth(text) / 2,
				bounds.getYCenter() - Theme::fontNormal.getHeight() + 1
			),
			&Theme::fontNormal,
			&Theme::magenta,
			text
		);
		
		text = L"0.5NM";
		
		renderer->renderString(
			Point(
				bounds.getXCenter() - Theme::fontSmall.getWidth(text) / 2,
				bounds.getYCenter()
			),
			&Theme::fontSmall,
			&Theme::fg1,
			text
		);
	}
	
	LateralRotaryControl::LateralRotaryControl() {
		setVariants({
			&seven
		});
		
		seven.setValue(RC::getInstance().getSettings().autopilot.headingDeg);
	}
	
	std::wstring_view LateralRotaryControl::variantIndexToTitle(uint8_t index) {
		switch (index) {
			default: return L"HDG";
		}
	}
	
	bool LateralRotaryControl::isVariantEditable(uint8_t index) {
		return index == 0;
	}
	
	void LateralRotaryControl::onRotate(bool clockwise, bool big) {
		SevenRotaryControl::onRotate(clockwise, big);
		
		RC::getInstance().getSettings().autopilot.headingDeg = static_cast<uint16_t>(seven.getValue());
		RC::getInstance().getSettings().autopilot.scheduleWrite();
		
		RC::getInstance().getCommunicationManager().enqueue(RemotePacketType::autopilot);
	}
	
	void LateralRotaryControl::onPress() {
		RotaryControl::onPress();
		
		const auto newMode = AutopilotLateralMode::hdg;
		
		RC::getInstance().getRemoteData().autopilot.lateralMode =
			RC::getInstance().getRemoteData().autopilot.lateralMode == newMode
			? AutopilotLateralMode::man
			: newMode;
		
		RC::getInstance().getCommunicationManager().enqueue(RemotePacketType::autopilot);
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
		}
	}
}
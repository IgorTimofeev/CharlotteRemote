#include "lateralRotaryControl.h"

#include "rc.h"

namespace pizda {
	void LateralRotaryControlStab::onRender(Renderer* renderer, const Bounds& bounds) {
		auto& rc = RC::getInstance();

		constexpr static uint8_t textLength = 8;
		wchar_t text[textLength];
		std::swprintf(text, textLength, L"%ls%d°", rc.getAircraftData().computed.autopilot.rollRad >= 0 ? L"+" : L"-", static_cast<uint16_t>(std::abs(toDegrees(rc.getAircraftData().computed.autopilot.rollRad))));

		renderer->renderString(
			Point(
				bounds.getXCenter() - Theme::fontNormal.getWidth(text) / 2,
				bounds.getYCenter() - Theme::fontNormal.getHeight() / 2
			),
			&Theme::fontNormal,
			&Theme::green1,
			text
		);
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
			default: return L"STAB";
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
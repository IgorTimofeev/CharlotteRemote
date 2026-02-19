#include "verticalRotaryControl.h"

#include "rc.h"

namespace pizda {
	void VerticalRotaryControlALT::onRender(Renderer* renderer, const Bounds& bounds) {
		auto text = std::to_wstring(static_cast<uint16_t>(
			RC::getInstance().getRemoteData().autopilot.altitudeHoldFt > 0
			? RC::getInstance().getRemoteData().autopilot.altitudeHoldFt
			: RC::getInstance().getAircraftData().computed.altitudeFt
		));
		
		const auto textWidth = Theme::fontNormal.getWidth(text);
		
		const auto lineWidth = textWidth;
		constexpr static uint8_t lineOffset = 0;
		
		const auto y = bounds.getYCenter() - (Theme::fontNormal.getHeight() + lineOffset + 1) / 2;
		
		renderer->renderString(
			Point(
				bounds.getXCenter() - textWidth / 2,
				y
			),
			&Theme::fontNormal,
			&Theme::ocean,
			text
		);
		
		renderer->renderHorizontalLine(
			Point(
				bounds.getXCenter() - lineWidth / 2,
				y + Theme::fontNormal.getHeight() + lineOffset
			),
			lineWidth,
			&Theme::ocean
		);
	}
	
	void VerticalRotaryControlStab::onRender(Renderer* renderer, const Bounds& bounds) {
		auto& rc = RC::getInstance();

		constexpr static uint8_t textLength = 8;
		wchar_t text[textLength];
		std::swprintf(text, textLength, L"%ls%d°", rc.getAircraftData().computed.autopilot.pitchRad >= 0 ? L"+" : L"-", static_cast<uint16_t>(std::abs(toDegrees(rc.getAircraftData().computed.autopilot.pitchRad))));

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
	
	VerticalRotaryControl::VerticalRotaryControl() {
		setVariants({
			&seven,
			&stab,
			&ALT
		});

		switch (RC::getInstance().getRemoteData().autopilot.verticalMode) {
			case AutopilotVerticalMode::stab:
				setVariantIndex(1);
				break;

			case AutopilotVerticalMode::alt:
				setVariantIndex(2);
				break;

			default:
				setVariantIndex(0);
				break;
		}

		seven.setValue(RC::getInstance().getSettings().autopilot.altitudeFt);
	}
	
	std::wstring_view VerticalRotaryControl::variantIndexToTitle(const uint8_t index) {
		switch (index) {
			case 0: return L"FLC";
			case 1: return L"STAB";
			default: return L"ALT";
		}
	}
	
	bool VerticalRotaryControl::isVariantEditable(const uint8_t index) {
		return index == 0;
	}
	
	void VerticalRotaryControl::onRotate(const bool clockwise, const bool big) {
		SevenRotaryControl::onRotate(clockwise, big);
		
		RC::getInstance().getSettings().autopilot.altitudeFt = static_cast<uint16_t>(seven.getValue());
		RC::getInstance().getSettings().autopilot.scheduleWrite();
		
		RC::getInstance().getTransceiver().enqueueAuxiliary(RemoteAuxiliaryPacketType::autopilot);
	}
	
	void VerticalRotaryControl::onPress() {
		RotaryControl::onPress();
		
		AutopilotVerticalMode newMode;
		
		switch (getVariantIndex()) {
			case 0: {
				newMode = AutopilotVerticalMode::flc;
				break;
			}
			case 1: {
				newMode = AutopilotVerticalMode::stab;
				RC::getInstance().getRemoteData().autopilot.altitudeHoldFt = RC::getInstance().getAircraftData().computed.altitudeFt;
				break;
			}
			default: {
				newMode = AutopilotVerticalMode::alt;
				RC::getInstance().getRemoteData().autopilot.altitudeHoldFt = RC::getInstance().getAircraftData().computed.altitudeFt;
				break;
			}
		}
		
		RC::getInstance().getRemoteData().autopilot.verticalMode =
			newMode == RC::getInstance().getRemoteData().autopilot.verticalMode
			? AutopilotVerticalMode::dir
			: newMode;
		
		RC::getInstance().getTransceiver().enqueueAuxiliary(RemoteAuxiliaryPacketType::autopilot);
	}
	
	void VerticalRotaryControl::onTick() {
		RotaryControl::onTick();
		
		switch (RC::getInstance().getAircraftData().raw.autopilot.verticalMode) {
			case AutopilotVerticalMode::dir:
				setBorderColor(nullptr);
				break;

			case AutopilotVerticalMode::stab:
				setBorderColor(getVariantIndex() == 1 ? &Theme::fg1 : &Theme::yellow);
				break;
				
			case AutopilotVerticalMode::flc:
				setBorderColor(getVariantIndex() == 0 ? &Theme::fg1 : &Theme::yellow);
				break;
				
			case AutopilotVerticalMode::alts:
				setBorderColor(getVariantIndex() == 0 ? &Theme::fg1 : &Theme::yellow);
				break;
				
			case AutopilotVerticalMode::alt:
				setBorderColor(getVariantIndex() == 2 ? &Theme::fg1 : &Theme::yellow);
				break;
		}
	}
}
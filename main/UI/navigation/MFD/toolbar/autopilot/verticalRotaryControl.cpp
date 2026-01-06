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
	
	void VerticalRotaryControlVNAV::onRender(Renderer* renderer, const Bounds& bounds) {
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
		
		text = L"1200";
		
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
	
	void VerticalRotaryControl::onPress() {
		RotaryControl::onPress();
		
		AutopilotVerticalMode newMode;
		
		switch (getVariantIndex()) {
			case 0: {
				newMode = AutopilotVerticalMode::flc;
				break;
			}
			case 1: {
				newMode = AutopilotVerticalMode::alt;
				RC::getInstance().getRemoteData().autopilot.altitudeHoldFt = RC::getInstance().getAircraftData().computed.altitudeFt;
				break;
			}
			default: {
				newMode = AutopilotVerticalMode::vnav;
				break;
			}
		}
		
		RC::getInstance().getRemoteData().autopilot.verticalMode =
			newMode == RC::getInstance().getRemoteData().autopilot.verticalMode
			? AutopilotVerticalMode::man
			: newMode;
		
		RC::getInstance().getPacketHandler().enqueue(RemotePacketType::autopilot);
	}
	
	void VerticalRotaryControl::onTick() {
		RotaryControl::onTick();
		
		setBorderColor(
			RC::getInstance().getAircraftData().raw.autopilot.verticalMode == AutopilotVerticalMode::man
			? nullptr
			: (
				RC::getInstance().getAircraftData().raw.autopilot.verticalMode == RC::getInstance().getRemoteData().autopilot.verticalMode
				? &Theme::fg1
				: &Theme::yellow
			)
		);
	}
}
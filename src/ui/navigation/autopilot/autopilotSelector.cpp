#include "autopilotSelector.h"

#include "ui/theme.h"

namespace pizdanc {
	AutopilotSelector::AutopilotSelector(uint8_t digitCount, const std::wstring_view& titleText, const std::wstring_view& buttonText) {
		const uint8_t marginValue = 10;

		// Background
		background.setPrimaryColor(&Theme::bg2);
		background.setCornerRadius(5);
		*this += &background;

		// Title
		title.setMargin(Margin(marginValue, marginValue, marginValue, 5));
		title.setHorizontalAlignment(Alignment::center);
		title.setPrimaryColor(&Theme::fg3);
		title.setFont(&Theme::fontNormal);
		title.setText(titleText);
		rows += &title;

		// Seven segment
		seven.setMargin(Margin(marginValue, 0, marginValue, 0));
		seven.setHorizontalAlignment(Alignment::center);
		seven.setDigitCount(digitCount);
		seven.setPrimaryColor(&Theme::bg4);
		seven.setSecondaryColor(&Theme::fg1);
		seven.setSegmentThickness(2);
		seven.setSegmentLength(10);
		rows += &seven;

		// Rotary knob
		knob.setMargin(Margin(marginValue));
		knob.setHorizontalAlignment(Alignment::center);
		knob.setPrimaryColor(&Theme::bg3);
		knob.setMiddleColor(&Theme::bg5);
		knob.setLineColor(&Theme::yellow);
		knob.setSize(Size(50, 50));
		rows += &knob;

		// Button
		button.setToggle(true);
		button.setHeight(24);
		button.setCornerRadius(5);
		button.setPrimaryColor(&Theme::bg3);
		button.setPressedPrimaryColor(&Theme::fg1);
		button.setSecondaryColor(&Theme::fg1);
		button.setPressedSecondaryColor(&Theme::bg2);
		button.setFont(&Theme::fontNormal);
		button.setText(buttonText);
		rows += &button;

		*this += &rows;
	}
}
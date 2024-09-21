#include "autopilotSelector.h"

namespace pizdanc {
	AutopilotSelector::AutopilotSelector(uint8_t digitCount, const wchar_t *titleText, const wchar_t *buttonText) {
		const uint8_t marginValue = 10;

		// Background
		background.setBackgroundColor(&Theme::bg2);
		background.setCornerRadius(5);
		*this += &background;

		// Title
		title.setMargin(Margin(marginValue, marginValue, marginValue, 5));
		title.setHorizontalAlignment(Alignment::Center);
		title.setForegroundColor(&Theme::fg3);
		title.setText(titleText);
		rows += &title;

		// Seven segment
		seven.setMargin(Margin(marginValue, 0, marginValue, 0));
		seven.setHorizontalAlignment(Alignment::Center);
		seven.setDigitCount(digitCount);
		seven.setBackgroundColor(&Theme::bg4);
		seven.setForegroundColor(&Theme::fg1);
		seven.setSegmentThickness(2);
		seven.setSegmentLength(10);
		rows += &seven;

		// Rotary knob
		knob.setMargin(Margin(marginValue));
		knob.setHorizontalAlignment(Alignment::Center);
		knob.setBackgroundColor(&Theme::bg3);
		knob.setCenterColor(&Theme::bg5);
		knob.setLineColor(&Theme::yellow);
		knob.setSize(Size(50, 50));
		rows += &knob;

		// Button
		button.setToggle(true);
		button.setHeight(24);
		button.setCornerRadius(5);
		button.setBackgroundColor(&Theme::bg3);
		button.setPressedBackground(&Theme::fg1);
		button.setForegroundColor(&Theme::fg1);
		button.setPressedForeground(&Theme::bg1);
		button.setText(buttonText);
		rows += &button;

		*this += &rows;
	}
}
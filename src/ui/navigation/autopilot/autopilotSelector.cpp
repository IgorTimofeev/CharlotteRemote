#include "autopilotSelector.h"

namespace pizdanc {
	AutopilotSelector::AutopilotSelector(uint8_t digitCount, const wchar_t *titleText, const wchar_t *buttonText) {
		const uint8_t marginValue = 10;

		// Background
		background.setBackgroundColor(&Theme::bg2);
		background.setCornerRadius(5);
		addChild(&background);

		// Title
		title.setMargin(Margin(marginValue, marginValue, marginValue, 5));
		title.setHorizontalAlignment(Alignment::Center);
		title.setForegroundColor(&Theme::fg3);
		title.setText(titleText);
		rows.addChild(&title);

		// Seven segment
		sevenSegment.setMargin(Margin(marginValue, 0, marginValue, 0));
		sevenSegment.setHorizontalAlignment(Alignment::Center);
		sevenSegment.setDigitCount(digitCount);
		sevenSegment.setBackgroundColor(&Theme::bg4);
		sevenSegment.setForegroundColor(&Theme::fg1);
		sevenSegment.setSegmentThickness(2);
		sevenSegment.setSegmentLength(10);
		sevenSegment.setValue(SevenSegment::dashes);
		rows.addChild(&sevenSegment);

		// Rotary knob
		rotaryKnob.setMargin(Margin(0, 0, 0, marginValue));
		rotaryKnob.setHorizontalAlignment(Alignment::Center);
		rotaryKnob.setBackgroundColor(&Theme::bg3);
		rotaryKnob.setCenterColor(&Theme::bg5);
		rotaryKnob.setLineColor(&Theme::yellow);
		rotaryKnob.setSize(Size(50, 50));
		rows.addChild(&rotaryKnob);

		// Button
		button.setToggle(true);
		button.setHeight(24);
		button.setCornerRadius(5);
		button.setBackgroundColor(&Theme::bg3);
		button.setPressedBackground(&Theme::fg1);
		button.setForegroundColor(&Theme::fg1);
		button.setPressedForeground(&Theme::bg1);
		button.setText(buttonText);
		rows.addChild(&button);

		addChild(&rows);
	}
}
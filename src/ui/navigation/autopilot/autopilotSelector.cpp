#include "autopilotSelector.h"

namespace pizdanc {
	AutopilotSelector::AutopilotSelector(const wchar_t *titleText) {
		background.setBackgroundColor(&Theme::bg2);
		background.setCornerRadius(5);
		addChild(&background);

		rows.setMargin(Margin(10));
		rows.setSpacing(20);

		// Seven segment
		setIndicatorStyle(sevenSegment);
		title.addChild(&sevenSegment);

		// Title
		title.setAlignment(Alignment::Center);
		title.setText(titleText);
		rows.addChild(&title);

		// Rotary knob
		setRotaryStyle(rotaryKnob);
		rotaryKnob.setAlignment(Alignment::Center);
		rows.addChild(&rotaryKnob);

		addChild(&rows);
	}

	void AutopilotSelector::setIndicatorStyle(SevenSegment &indicator) {
		indicator.setBackgroundColor(&Theme::bg4);
		indicator.setForegroundColor(&Theme::fg1);
		indicator.setDigitCount(4);
		indicator.setSegmentThickness(2);
		indicator.setSegmentLength(10);
	}

	void AutopilotSelector::setRotaryStyle(RotaryKnob &rotaryKnob) {
		rotaryKnob.setBackgroundColor(&Theme::bg3);
		rotaryKnob.setCenterColor(&Theme::bg5);
		rotaryKnob.setLineColor(&Theme::yellow);
		rotaryKnob.setSize(Size(50, 50));
	}
}
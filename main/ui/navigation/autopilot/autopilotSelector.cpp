#include "autopilotSelector.h"
#include "../../theme.h"

namespace pizdanc {
	AutopilotSelector::AutopilotSelector(uint8_t digitCount, const std::wstring_view& titleText, const std::wstring_view& buttonText) {
		const uint8_t marginValue = 10;

		// Background
		background.setFillColor(&Theme::bg2);
		background.setCornerRadius(5);
		*this += &background;

		// Button
		button.setToggle(true);
		button.setWidth(30);
		button.setCornerRadius(5);

		button.setDefaultBackgroundColor(&Theme::bg3);
		button.setPressedBackgroundColor(&Theme::fg1);

		button.setDefaultTextColor(&Theme::fg1);
		button.setPressedTextColor(&Theme::bg2);

		button.setFont(&Theme::fontNormal);
		button.setText(buttonText);

		row.setFit(&button);
		row += &button;

		// Titler
		titler.setMargin(Margin(marginValue));
		titler.setVerticalAlignment(Alignment::center);
		titler.getTitle().setTextColor(&Theme::fg3);
		titler.getTitle().setFont(&Theme::fontNormal);
		titler.getTitle().setText(titleText);
		row += &titler;

		// Seven segment
		seven.setVerticalAlignment(Alignment::center);
		seven.setDigitCount(digitCount);
		seven.setPrimaryColor(&Theme::bg4);
		seven.setSecondaryColor(&Theme::fg1);
		seven.setSegmentThickness(2);
		seven.setSegmentLength(10);
		titler += &seven;

		// Rotary knob
		knob.setMargin(Margin(marginValue));
		knob.setVerticalAlignment(Alignment::center);
		knob.setBackgroundColor(&Theme::bg3);
		knob.setMiddleColor(&Theme::bg5);
		knob.setLineColor(&Theme::yellow);
		knob.setSize(Size(46, 46));

		row.setFit(&knob);
		row += &knob;

		*this += &row;
	}
}
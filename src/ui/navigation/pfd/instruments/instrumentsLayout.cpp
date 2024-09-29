#include "instrumentsLayout.h"
#include "../../../rc_application.h"

namespace pizdanc {
	InstrumentsLayout::InstrumentsLayout() {
		*this += &_backgroundRect;

		// Rows
		_rows.setMargin(Margin(8, 4, 8, 4));
		_rows.setSpacing(10);

		// Throttle
		_throttleRow.setOrientation(Orientation::Horizontal);
		_throttleRow.setSpacing(10);
		_throttleRow.setHorizontalAlignment(Alignment::Center);

		_throttle1Indicator.setSize(Size(8, 44));
		_throttleRow += &_throttle1Indicator;

		_throttle2Indicator.setSize(_throttle1Indicator.getSize());
		_throttleRow += &_throttle2Indicator;

		_rows += &_throttleTitle;

		// Controls
		_rows += &_controlsTitle;

		// Pitch
		_pitchTrimIndicator.setSize(Size(20, 40));
		_pitchTrimIndicator.setSuggestedMinimum(0.4);
		_pitchTrimIndicator.setSuggestedMaximum(0.6);
		_pitchTrimIndicator.setValue(0.5);
		_rows += &_pitchTitle;

		*this += &_rows;
	}

	void InstrumentsLayout::tick() {
		auto& app = RCApplication::getInstance();

		// Engine
		_throttle1Indicator.setValue(app.getThrottle1Interpolator().getValue());
		_throttle2Indicator.setValue(app.getThrottle2Interpolator().getValue());

		// Trim
		_pitchTrimIndicator.setValue(app.getElevatorTrimInterpolator().getValue());
	}
}
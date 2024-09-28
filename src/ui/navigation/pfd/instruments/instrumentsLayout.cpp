#include "instrumentsLayout.h"
#include "../../../rc_application.h"

namespace pizdanc {
	InstrumentsLayout::InstrumentsLayout() {
		// Background
		*this += &_backgroundRect;

		// Rows
		_rows.setMargin(Margin(5));
		_rows.setSpacing(10);

		// Pitch
		_pitchTrimIndicator.setSize(Size(20, 50));
		_pitchTrimIndicator.setSuggestedMinimum(0.4);
		_pitchTrimIndicator.setSuggestedMaximum(0.6);
		_pitchTrimIndicator.setValue(0.5);
		_rows += &_pitchTitle;

		*this += &_rows;
	}

	void InstrumentsLayout::tick() {
		auto& app = RCApplication::getInstance();

		_pitchTrimIndicator.setValue(app.getElevatorTrimInterpolator().getValue());
	}
}
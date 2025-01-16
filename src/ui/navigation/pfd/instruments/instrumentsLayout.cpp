#include "instrumentsLayout.h"
#include "../../../rc_application.h"

namespace pizdanc {
	InstrumentsLayout::InstrumentsLayout() {
		*this += &_backgroundRect;

		// Rows
		_rows.setMargin(Margin(8, 4, 8, 4));
		_rows.setSpacing(10);

		// Throttle
		Theme::apply(_throttleTitle);

		_throttleRow.setOrientation(Orientation::Horizontal);
		_throttleRow.setSpacing(10);
		_throttleRow.setHorizontalAlignment(Alignment::Center);

		_throttle1Indicator.setSize(Size(8, 44));
		_throttleRow += &_throttle1Indicator;

		_throttle2Indicator.setSize(_throttle1Indicator.getSize());
		_throttleRow += &_throttle2Indicator;

		_rows += &_throttleTitle;

		// Controls
		Theme::apply(_controlsTitle);
		_rows += &_controlsTitle;

		// Trim
		_trimRow.setOrientation(Orientation::Horizontal);
		_trimRow.setSpacing(5);

		// Elevator
		Theme::apply(_elevatorTrimTitle);
		_elevatorTrimIndicator.setSize(Size(5, 28));
		_elevatorTrimIndicator.setSuggestedMinimum(0.4);
		_elevatorTrimIndicator.setSuggestedMaximum(0.6);
		_trimRow += &_elevatorTrimTitle;

		// Ailerons & rudder
		_aileronsAndRudderRows.setSpacing(3);

		// Ailerons
		Theme::apply(_aileronsTrimTitle);
		_aileronsTrimIndicator.setOrientation(Orientation::Horizontal);
		_aileronsTrimIndicator.setSize(Size(28, 5));
		_aileronsTrimIndicator.setSuggestedMinimum(0.4);
		_aileronsTrimIndicator.setSuggestedMaximum(0.6);
		_aileronsAndRudderRows += &_aileronsTrimTitle;

		// Rudder
		Theme::apply(_rudderTrimTitle);
		_rudderTrimIndicator.setOrientation(_aileronsTrimIndicator.getOrientation());
		_rudderTrimIndicator.setSize(_aileronsTrimIndicator.getSize());
		_rudderTrimIndicator.setSuggestedMinimum(_aileronsTrimIndicator.getSuggestedMinimum());
		_rudderTrimIndicator.setSuggestedMaximum(_aileronsTrimIndicator.getSuggestedMaximum());
		_aileronsAndRudderRows += &_rudderTrimTitle;

		_trimRow += &_aileronsAndRudderRows;

		_rows += &_trimRow;

		*this += &_rows;
	}

	void InstrumentsLayout::tick() {
		Layout::tick();

		auto app = &RCApplication::getInstance();

		// Engine
		_throttle1Indicator.setValue(app->getThrottle1Interpolator().getValue());
		_throttle2Indicator.setValue(app->getThrottle2Interpolator().getValue());

		// Trim
		_elevatorTrimIndicator.setValue(app->getElevatorTrimInterpolator().getValue());
		_aileronsTrimIndicator.setValue(app->getAileronsTrimInterpolator().getValue());
		_rudderTrimIndicator.setValue(app->getRudderTrimInterpolator().getValue());
	}
}
#include "instrumentsLayout.h"
#include "../../../rc_application.h"

namespace pizdanc {
	InstrumentsLayout::InstrumentsLayout() {
		*this += &_backgroundRect;

		// Row
		_row.setOrientation(Orientation::horizontal);
		_row.setSpacing(15);
		_row.setHorizontalAlignment(Alignment::center);
		_row.setMargin(Margin(8, 4, 8, 4));

		// Throttle
		Theme::apply(&_throttleTitle);

		const uint8_t contentHeight = InstrumentsLayout::panelSize - _row.getMargin().getVertical() - _throttleTitle.getSpacing() - _throttleTitle.getTitle().getFont()->getHeight();

		_throttleRow.setOrientation(Orientation::horizontal);
		_throttleRow.setSpacing(12);

		_throttle1Indicator.setSize(Size(7, contentHeight));
		_throttleRow += &_throttle1Indicator;

		_throttle2Indicator.setSize(_throttle1Indicator.getSize());
		_throttleRow += &_throttle2Indicator;

		_row += &_throttleTitle;

		// Controls
		Theme::apply(&_controlsTitle);
		_row += &_controlsTitle;

		// Elevator trim
		Theme::apply(&_elevatorTrimTitle);
		_elevatorTrimIndicator.setSize(Size(5, contentHeight));
		_elevatorTrimIndicator.setSuggestedMinimum(0.4);
		_elevatorTrimIndicator.setSuggestedMaximum(0.6);
		_row += &_elevatorTrimTitle;

		*this += &_row;
	}

	void InstrumentsLayout::onTick() {
		Layout::onTick();

		auto app = &RCApplication::getInstance();

		// Throttle
		_throttle1Indicator.setValue(app->getThrottle1Interpolator().getValue());
		_throttle2Indicator.setValue(app->getThrottle2Interpolator().getValue());

		// Trim
		_elevatorTrimIndicator.setValue(app->getElevatorTrimInterpolator().getValue());
	}
}
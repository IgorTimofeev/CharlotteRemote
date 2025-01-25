#include "instrumentsLayout.h"
#include "../../../../rc.h"

namespace pizdanc {
	InstrumentsLayout::InstrumentsLayout() {
		_backgroundRect.setFillColor(&Theme::bg1);
		*this += &_backgroundRect;

		// Titles
		_titlesRect.setFillColor(&Theme::bg2);
		_titlesRect.setHeight(12);
		_titlesRect.setVerticalAlignment(Alignment::start);
		*this += &_titlesRect;

		// Row
		_row.setOrientation(Orientation::horizontal);
		_row.setSpacing(15);
		_row.setHorizontalAlignment(Alignment::center);
		_row.setMargin(Margin(8, 1, 8, 0));

		// Throttle
		Theme::apply(&_throttleTitle);

		const uint8_t contentHeight = InstrumentsLayout::panelSize - _row.getMargin().getTop() - _throttleTitle.getSpacing() - _throttleTitle.getTitle().getFont()->getHeight();

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
		_elevatorTrimIndicator.setSize(Size(5, contentHeight - 4));
		_elevatorTrimIndicator.setSuggestedMinimum(0.4);
		_elevatorTrimIndicator.setSuggestedMaximum(0.6);
		_row += &_elevatorTrimTitle;

		// Battery
		Theme::apply(&_batteryIndicatorTitle);

		_batteryIndicatorController.setSize(Size(30, 14));
		_batteryIndicatorAircraft.setSize(_batteryIndicatorController.getSize());

		_batteryIndicatorRows.setSpacing(5);
		_batteryIndicatorRows += &_batteryIndicatorController;
		_batteryIndicatorRows += &_batteryIndicatorAircraft;

		_row += &_batteryIndicatorTitle;

		*this += &_row;
	}

	void InstrumentsLayout::onTick() {
		Layout::onTick();

		auto rc = &RC::getInstance();

		// Throttle
		_throttle1Indicator.setValue(rc->getThrottle1Interpolator().getValue());
		_throttle2Indicator.setValue(rc->getThrottle2Interpolator().getValue());

		// Trim
		_elevatorTrimIndicator.setValue(rc->getElevatorTrimInterpolator().getValue());

		// Battery
		_batteryIndicatorController.setValue(rc->getBatteryCharge());
		_batteryIndicatorAircraft.setValue(rc->getBatteryCharge() * 0.8f);
	}
}
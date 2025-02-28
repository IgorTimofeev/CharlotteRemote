#include "instrumentsLayout.h"
#include "../../../../rc.h"

namespace pizda {
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

		_batteryIndicatorController.setSize(Size(28, 12));
		_batteryIndicatorAircraft.setSize(_batteryIndicatorController.getSize());

		_batteryIndicatorRows.setSpacing(5);
		_batteryIndicatorRows += &_batteryIndicatorController;
		_batteryIndicatorRows += &_batteryIndicatorAircraft;

		_row += &_batteryIndicatorTitle;

		*this += &_row;
	}

	void InstrumentsLayout::onTick() {
		Layout::onTick();

		auto& rc = RC::getInstance();

		if (rc.getEncoder().wasInterrupted()) {
			if (rc.getEncoder().getRotation() != 0) {
				_throttle1Indicator.setValue(yoba::clamp(_throttle1Indicator.getValue() + rc.getEncoder().getRotation() / 100.f, 0.f, 1.f));
				_throttle2Indicator.setValue(_throttle1Indicator.getValue());

				rc.getEncoder().setRotation(0);
			}

			rc.getEncoder().acknowledgeInterrupt();
		}

		// Trim
		_elevatorTrimIndicator.setValue(rc.getElevatorTrimInterpolator().getValue());

		// Battery
		_batteryIndicatorController.setValue(rc.getBattery().getVoltage());
		_batteryIndicatorAircraft.setValue(rc.getBattery().getVoltage() * 1 / 3);
	}
}
#include "mainToolbar.h"

#include "rc.h"
#include <resources/images.h>
#include "UI/navigation/MFD/toolbar/toolbar.h"

namespace pizda {
	MainToolbar::MainToolbar() {
		// ----------------------------- Left -----------------------------
		
		leftRow.setOrientation(Orientation::horizontal);
		leftRow.setHorizontalAlignment(Alignment::start);
		*this += &leftRow;

		// Throttle
		leftRow += &_throttleIndicatorLayout;

		// Controls
		ToolbarSection::setDefaultMargin(&_flightControlsIndicator, 4);
		_flightControlsSection.setFocusable(false);
		leftRow += &_flightControlsSection;
		
		// ----------------------------- Right -----------------------------
		
		rightRow.setOrientation(Orientation::horizontal);
		rightRow.setHorizontalAlignment(Alignment::end);
		*this += &rightRow;
		
		// Radio
		ToolbarSection::setDefaultMargin(&_radio, 7);
		_radioSection.setFocusable(false);
		rightRow += &_radioSection;
		
		// Battery
		_batteryRows.setSpacing(3);
		ToolbarSection::setDefaultMargin(&_batteryRows, 4);
		_batteryRows += &_batteryIndicatorRC;
		_batteryRows += &_batteryIndicatorAC;
		_batterySection.setFocusable(false);
		rightRow += &_batterySection;
	}

	void MainToolbar::onTick() {
		Layout::onTick();

		auto& rc = RC::getInstance();
		
		// ----------------------------- Battery -----------------------------

		// Remote
		_batteryIndicatorRC.setVoltage(rc.getBattery().getVoltage());
		_batteryIndicatorRC.setCharge(rc.getBattery().getCharge());

		// Aircraft
		_batteryIndicatorAC.setVoltage(
			rc.getCommunicationManager().isConnected()
			? rc.getAircraftData().raw.batteryVoltageMV
			: BatteryIndicator::voltageNotAvailable
		);

		_batteryIndicatorAC.setCharge(
			rc.getCommunicationManager().isConnected()
			? static_cast<uint8_t>(
				(rc.getAircraftData().raw.batteryVoltageMV - config::battery::aircraft::voltageMin)
				* 0xFF
				/ (config::battery::aircraft::voltageMax - config::battery::aircraft::voltageMin)
			)
			: 0
		);
	}
}
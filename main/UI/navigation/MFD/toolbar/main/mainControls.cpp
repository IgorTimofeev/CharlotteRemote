#include "mainControls.h"

#include "rc.h"
#include <resources/images.h>

namespace pizda {
	MainControls::MainControls() {
		setHeight(36);
		
		// Throttle
		row += &_throttleIndicatorLayout;

		// Controls
		_flightControlsIndicator.setMargin(Margin(0, 4, 0, 0));
		_flightControlsLayout.setFocusable(false);
		row += &_flightControlsLayout;
		
		// Radio
		_radio.setMargin(Margin(0, 6, 0, 0));
		_radioLayout.setFocusable(false);
		row += &_radioLayout;
		
		// Battery
		_batteryRows.setSpacing(2);
		_batteryRows.setMargin(Margin(0, 3, 0, 0));
		_batteryRows += &_batteryIndicatorRC;
		_batteryRows += &_batteryIndicatorAC;
		_batteryLayout.setFocusable(false);
		row += &_batteryLayout;
	}

	void MainControls::onTick() {
		Layout::onTick();

		auto& rc = RC::getInstance();
		
		// Battery
		_batteryIndicatorRC.setVoltage(rc.getBattery().getVoltage());
		_batteryIndicatorRC.setCharge(rc.getBattery().getCharge());

		_batteryIndicatorAC.setVoltage(14800);
		_batteryIndicatorAC.setCharge(0xFF);
	}
}
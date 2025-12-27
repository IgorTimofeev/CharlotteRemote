#include "mainToolbar.h"

#include "rc.h"
#include <resources/images.h>
#include "UI/navigation/MFD/toolbar/toolbar.h"

namespace pizda {
	MainToolbar::MainToolbar() {
		setHeight(36);
		
		// Throttle
		row += &_throttleIndicatorLayout;

		// Controls
		ToolbarSection::setDefaultMargin(&_flightControlsIndicator, 4);
		_flightControlsSection.setFocusable(false);
		_flightControlsSection.setMargin(Margin(0, 0, 11, 0));
		row += &_flightControlsSection;
		
		// Radio
		ToolbarSection::setDefaultMargin(&_radio, 6);
		_radioSection.setFocusable(false);
		_radioSection.setMargin(Margin(11, 0, 0, 0));
		row += &_radioSection;
		
		// Battery
		_batteryRows.setSpacing(3);
		ToolbarSection::setDefaultMargin(&_batteryRows, 3);
		_batteryRows += &_batteryIndicatorRC;
		_batteryRows += &_batteryIndicatorAC;
		_batterySection.setFocusable(false);
		row += &_batterySection;
	}

	void MainToolbar::onTick() {
		Layout::onTick();

		auto& rc = RC::getInstance();
		
		// Battery
		_batteryIndicatorRC.setVoltage(rc.getBattery().getVoltage());
		_batteryIndicatorRC.setCharge(rc.getBattery().getCharge());

		_batteryIndicatorAC.setVoltage(14800);
		_batteryIndicatorAC.setCharge(0xFF);
	}
}
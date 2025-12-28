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
		_flightControlsSection.setMargin(Margin(0, 0, 16, 0));
		row += &_flightControlsSection;
		
		// Radio
		ToolbarSection::setDefaultMargin(&_radio, 7);
		_radioSection.setFocusable(false);
		_radioSection.setMargin(Margin(16, 0, 0, 0));
		row += &_radioSection;
		
		// Battery
		_batteryRows.setSpacing(3);
		ToolbarSection::setDefaultMargin(&_batteryRows, 4);
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

		_batteryIndicatorAC.setVoltage(static_cast<uint16_t>(rc.getAircraftData().computed.batteryVoltageV * 1000));
		_batteryIndicatorAC.setCharge(_batteryIndicatorAC.getVoltage() * 0xFF / 14800);
	}
}
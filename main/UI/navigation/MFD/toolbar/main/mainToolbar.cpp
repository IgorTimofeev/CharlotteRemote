#include "mainToolbar.h"

#include "rc.h"
#include <resources/images.h>
#include "UI/navigation/MFD/toolbar/toolbar.h"

namespace pizda {
	MainToolbar::MainToolbar() {
		setHeight(36);
		
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
		
		// Battery
		_batteryIndicatorRC.setVoltage(rc.getBattery().getVoltage());
		_batteryIndicatorRC.setCharge(rc.getBattery().getCharge());

		_batteryIndicatorAC.setVoltage(rc.getPacketHandler().isConnected() ? static_cast<uint16_t>(rc.getAircraftData().raw.batteryVoltageV * 1000) : 0);
		_batteryIndicatorAC.setCharge(_batteryIndicatorAC.getVoltage() * 0xFF / 14800);
	}
}
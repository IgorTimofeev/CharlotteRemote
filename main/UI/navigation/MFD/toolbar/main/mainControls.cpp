#include "mainControls.h"
#include <rc.h>
#include <resources/images.h>

namespace pizda {
	MainControls::MainControls() {
		setHeight(46);

		// Throttle
		row += &_throttleIndicatorLayout;

		// Controls
		_controlsRows.setSpacing(5);
		_controlsRows += &_flapsAndSpoilersIndicator;
		_controlsRows += &_landingGearImageView;
		row += &_controlsLayout;

		// Battery
		_batteryIndicatorController.setSize(Size(32, 12));
		_batteryIndicatorAircraft.setSize(_batteryIndicatorController.getSize());

		_batteryIndicatorRows.setSpacing(5);
		_batteryIndicatorRows += &_batteryIndicatorController;
		_batteryIndicatorRows += &_batteryIndicatorAircraft;
		row += &_batteryLayout;

		// Radio
		row += &_radioLayout;
	}

	void MainControls::onTick() {
		Layout::onTick();

		auto& rc = RC::getInstance();
		const auto& settings = rc.getSettings();

		// Controls
		_landingGearImageView.setImage(
			settings.controls.landingGear
			? reinterpret_cast<const Image*>(&resources::Images::MFDLandingGearExtended)
			: reinterpret_cast<const Image*>(&resources::Images::MFDLandingGearRetracted)
		);

		// Battery
		_batteryIndicatorController.setVoltage(rc.getBattery().getVoltage());
		_batteryIndicatorController.setCharge(rc.getBattery().getCharge());

		_batteryIndicatorAircraft.setVoltage(28000);
		_batteryIndicatorAircraft.setCharge(0xFF);
	}
}
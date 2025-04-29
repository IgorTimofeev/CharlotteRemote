#include "mainControls.h"
#include "../../../../rc.h"
#include "../../../../resources/images.h"

namespace pizda {
	MainControls::MainControls() {
		setHeight(48);

		// Throttle
		row += &_throttleIndicatorLayout;

		// Controls
		_controlsRows.setSpacing(5);
		_controlsRows += &_flapsAndSpoilersIndicator;
		_controlsRows += &_landingGearImageView;
		row += &_controlsLayout;

		// Elevator trim
		_elevatorTrimIndicator.setWidth(5);
		_elevatorTrimIndicator.setSuggestedMinimum(0xFFFF * 40 / 100);
		_elevatorTrimIndicator.setSuggestedMaximum(0xFFFF * 60 / 100);
		row += &_elevatorTrimLayout;

		// Battery
		_batteryIndicatorController.setSize(Size(32, 12));
		_batteryIndicatorAircraft.setSize(_batteryIndicatorController.getSize());

		_batteryIndicatorRows.setSpacing(5);
		_batteryIndicatorRows += &_batteryIndicatorController;
		_batteryIndicatorRows += &_batteryIndicatorAircraft;
		row += &_batteryLayout;
	}

	void MainControls::onTick() {
		Layout::onTick();

		auto& rc = RC::getInstance();
		auto& settings = rc.getSettings();

		// Controls
		_landingGearImageView.setImage(
			settings.controls.landingGear
			? reinterpret_cast<const Image*>(&resources::Images::MFDLandingGearExtended)
			: reinterpret_cast<const Image*>(&resources::Images::MFDLandingGearRetracted)
		);

		// Trim
		_elevatorTrimIndicator.setValue(settings.controls.elevatorTrim);

		// Battery
		_batteryIndicatorController.setVoltage(rc.getBattery().getVoltage());
		_batteryIndicatorController.setCharge(rc.getBattery().getCharge());

		_batteryIndicatorAircraft.setVoltage(28000);
		_batteryIndicatorAircraft.setCharge(0xFF);
	}
}
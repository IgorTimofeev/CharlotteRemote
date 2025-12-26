#include "autopilotControls.h"
#include "rc.h"

namespace pizda {
	using namespace YOBA;

	AutopilotControls::AutopilotControls() {
		auto& settings = RC::getInstance().getSettings();
		setHeight(36);

//		row.setSpacing(6);

		// FD
		flightDirectorsButton.setSize(Size(24, 18));
		flightDirectorsButton.setMargin(Margin(0, 0, 8, 0));
		flightDirectorsButton.setVerticalAlignment(Alignment::center);
		flightDirectorsButton.setToggle(true);
		flightDirectorsButton.setActive(settings.interface.MFD.PFD.flightDirectors);
		
		flightDirectorsButton.isActiveChanged += [this, &settings] {
			settings.interface.MFD.PFD.flightDirectors = flightDirectorsButton.isActive();
			settings.interface.scheduleWrite();
		};
		
		row += &flightDirectorsButton;

		// Speed
		speed.seven.setValue(settings.autopilot.speedKt);
		speed.button.setActive(settings.autopilot.autoThrottle);

		speed.rotated += [this, &settings] {
			settings.autopilot.speedKt = speed.seven.getValue();
			settings.autopilot.scheduleWrite();
		};

		speed.button.isActiveChanged += [this, &settings] {
			settings.autopilot.autoThrottle = speed.button.isActive();
			settings.autopilot.scheduleWrite();
		};

		row += &speed;

		// Heading
		heading.seven.setValue(settings.autopilot.headingDeg);
		heading.button.setActive(settings.autopilot.headingHold);

		heading.rotated += [this, &settings] {
			settings.autopilot.headingDeg = heading.seven.getValue();
			settings.autopilot.scheduleWrite();
		};

		heading.button.isActiveChanged += [this, &settings] {
			settings.autopilot.headingHold = heading.button.isActive();
			settings.autopilot.scheduleWrite();
		};

		row += &heading;

		// Altitude
		altitude.seven.setValue(settings.autopilot.altitudeFt);
		altitude.button.setActive(settings.autopilot.levelChange);

		altitude.rotated += [this, &settings] {
			settings.autopilot.altitudeFt = altitude.seven.getValue();
			settings.autopilot.scheduleWrite();
		};

		altitude.button.isActiveChanged += [this, &settings] {
			settings.autopilot.levelChange = altitude.button.isActive();
			settings.autopilot.scheduleWrite();
		};

		row += &altitude;
		
		// LNAV
		LNAVButton.setSize(flightDirectorsButton.getSize());
		LNAVButton.setMargin(Margin(8, 0, 0, 0));
		LNAVButton.setVerticalAlignment(Alignment::center);
		LNAVButton.setToggle(true);
		LNAVButton.setActive(false);
		
		row += &LNAVButton;
	}
}
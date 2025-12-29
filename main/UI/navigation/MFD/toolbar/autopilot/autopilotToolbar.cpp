#include "autopilotToolbar.h"
#include "rc.h"

namespace pizda {
	AutopilotToolbar::AutopilotToolbar() {
		setHeight(36);
		
		row.setSpacing(1);
		row.setMargin(Margin(0, 0, (21 + Toolbar::contentHorizontalMargin * 2) / 2, 0));
		
		auto& settings = RC::getInstance().getSettings();

		// FD
		flightDirectorsButton.setActive(settings.interface.MFD.PFD.flightDirector);
		
		flightDirectorsButton.isActiveChanged += [this, &settings] {
			settings.interface.MFD.PFD.flightDirector = flightDirectorsButton.isActive();
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
	}
}
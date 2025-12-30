#include "autopilotToolbar.h"
#include "rc.h"

namespace pizda {
	AutopilotToolbar::AutopilotToolbar() {
		setHeight(36);
		
//		row.setSpacing(1);
		row.setMargin(Margin(0, 0, (21 + Toolbar::contentHorizontalMargin * 2) / 2, 0));
		
		auto& settings = RC::getInstance().getSettings();

		// FD
		flightDirectorButton.setActive(settings.interface.MFD.PFD.flightDirector);
		
		flightDirectorButton.isActiveChanged += [this, &settings] {
			settings.interface.MFD.PFD.flightDirector = flightDirectorButton.isActive();
			settings.interface.scheduleWrite();
		};
		
		row += &flightDirectorButton;

		// Speed
		speed.seven.setValue(settings.autopilot.speedKt);
		speed.setBottomActive(settings.autopilot.autoThrottle);

		speed.rotated += [this, &settings] {
			settings.autopilot.speedKt = speed.seven.getValue();
			settings.autopilot.scheduleWrite();
		};

		speed.bottomActiveChanged += [this, &settings] {
			settings.autopilot.autoThrottle = speed.isBottomActive();
			settings.autopilot.scheduleWrite();
		};
		
		row += &speed;

		// Heading
		heading.seven.setValue(settings.autopilot.headingDeg);
		heading.setBottomActive(settings.autopilot.headingHold);

		heading.rotated += [this, &settings] {
			settings.autopilot.headingDeg = heading.seven.getValue();
			settings.autopilot.scheduleWrite();
		};

		heading.bottomActiveChanged += [this, &settings] {
			settings.autopilot.headingHold = heading.isBottomActive();
			settings.autopilot.scheduleWrite();
		};
		
		row += &heading;

		// Altitude
		altitude.seven.setValue(settings.autopilot.altitudeFt);
		altitude.setBottomActive(settings.autopilot.levelChange);

		altitude.rotated += [this, &settings] {
			settings.autopilot.altitudeFt = altitude.seven.getValue();
			settings.autopilot.scheduleWrite();
		};

		altitude.bottomActiveChanged += [this, &settings] {
			settings.autopilot.levelChange = altitude.isBottomActive();
			settings.autopilot.scheduleWrite();
		};
		
		row += &altitude;
	}
}
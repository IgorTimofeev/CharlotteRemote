#include "autopilotToolbar.h"
#include "rc.h"
#include "resources/sounds.h"

namespace pizda {
	AutopilotToolbar::AutopilotToolbar() {
		setHeight(36);
		
		auto& rc = RC::getInstance();
		auto& settings = rc.getSettings();

		// FD
		flightDirectorButton.setActive(settings.interface.MFD.PFD.flightDirector);
		
		flightDirectorButton.isActiveChanged += [this, &settings] {
			settings.interface.MFD.PFD.flightDirector = flightDirectorButton.isActive();
			settings.interface.scheduleWrite();
		};
		
		row += &flightDirectorButton;

		// Speed
		speed.seven.setValue(settings.autopilot.speedKt);
		speed.setActive(settings.autopilot.autoThrottle);

		speed.rotated += [this, &settings] {
			settings.autopilot.speedKt = speed.seven.getValue();
			settings.autopilot.scheduleWrite();
		};

		speed.isActiveChanged += [this, &settings] {
			settings.autopilot.autoThrottle = speed.isActive();
			settings.autopilot.scheduleWrite();
		};
		
		row += &speed;

		// Heading
		heading.seven.setValue(settings.autopilot.headingDeg);
		heading.setActive(settings.autopilot.headingHold);

		heading.rotated += [this, &settings] {
			settings.autopilot.headingDeg = heading.seven.getValue();
			settings.autopilot.scheduleWrite();
		};

		heading.isActiveChanged += [this, &settings] {
			settings.autopilot.headingHold = heading.isActive();
			settings.autopilot.scheduleWrite();
		};
		
		row += &heading;

		// Altitude
		altitude.seven.setValue(settings.autopilot.altitudeFt);
		altitude.setActive(settings.autopilot.levelChange);

		altitude.rotated += [this, &settings] {
			settings.autopilot.altitudeFt = altitude.seven.getValue();
			settings.autopilot.scheduleWrite();
		};

		altitude.isActiveChanged += [this, &settings] {
			settings.autopilot.levelChange = altitude.isActive();
			settings.autopilot.scheduleWrite();
		};
		
		row += &altitude;
		
		// A/P
		engageButton.setActive(rc.getRemoteData().raw.autopilotEngaged);
		
		engageButton.isActiveChanged += [this, &rc] {
			rc.getRemoteData().raw.autopilotEngaged = engageButton.isActive();
			
			if (engageButton.isActive()) {
				rc.getSpeaker().play(resources::sounds::autopilotEngaged);
			}
			else {
				rc.getSpeaker().play(resources::sounds::autopilotDisengaged);
			}
		};
		
		row += &engageButton;
	}
}
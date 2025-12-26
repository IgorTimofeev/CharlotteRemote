#include "autopilotControls.h"
#include "rc.h"

namespace pizda {
	using namespace YOBA;

	AutopilotControls::AutopilotControls() {
		auto& settings = RC::getInstance().getSettings();
		setHeight(38);

		row.setSpacing(6);

		// FD
		FDButton.setSize(Size(26, 20));
		FDButton.setVerticalAlignment(Alignment::center);
		FDButton.setToggle(true);
		FDButton.setActive(settings.interface.MFD.PFD.flightDirectors);

		FDButton.isActiveChanged += [this, &settings] {
			settings.interface.MFD.PFD.flightDirectors = FDButton.isActive();
			settings.interface.scheduleWrite();
		};
		
		row += &FDButton;

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
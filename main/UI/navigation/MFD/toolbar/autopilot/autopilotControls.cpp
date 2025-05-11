#include "autopilotControls.h"
#include "../../../../../rc.h"

namespace pizda {
	using namespace YOBA;

	AutopilotControls::AutopilotControls() {
		auto& settings = RC::getInstance().getSettings();

		setHeight(52);

		row.setSpacing(8);

		// FD
		FDButton.setSize(Size(26, 20));
		FDButton.setVerticalAlignment(Alignment::center);
		FDButton.setCheckMode(ButtonCheckMode::toggle);
		FDButton.setChecked(settings.interface.MFD.PFD.flightDirectors);

		FDButton.isCheckedChanged += [this, &settings] {
			settings.interface.MFD.PFD.flightDirectors = FDButton.isChecked();
			settings.interface.scheduleWrite();
		};
		
		row += &FDButton;

		// Speed
		speed.seven.setValue(settings.autopilot.speedKt);
		speed.button.setChecked(settings.autopilot.autoThrottle);

		speed.rotated += [this, &settings] {
			settings.autopilot.speedKt = speed.seven.getValue();
			settings.autopilot.scheduleWrite();
		};

		speed.button.isCheckedChanged += [this, &settings] {
			settings.autopilot.autoThrottle = speed.button.isChecked();
			settings.autopilot.scheduleWrite();
		};

		speedLayout.setFocusable(false);
		row += &speedLayout;

		// Heading
		heading.seven.setValue(settings.autopilot.headingDeg);
		heading.button.setChecked(settings.autopilot.headingHold);

		heading.rotated += [this, &settings] {
			settings.autopilot.headingDeg = heading.seven.getValue();
			settings.autopilot.scheduleWrite();
		};

		heading.button.isCheckedChanged += [this, &settings] {
			settings.autopilot.headingHold = heading.button.isChecked();
			settings.autopilot.scheduleWrite();
		};

		headingLayout.setFocusable(false);
		row += &headingLayout;

		// Altitude
		altitude.seven.setValue(settings.autopilot.altitudeFt);
		altitude.button.setChecked(settings.autopilot.levelChange);

		altitude.rotated += [this, &settings] {
			settings.autopilot.altitudeFt = altitude.seven.getValue();
			settings.autopilot.scheduleWrite();
		};

		altitude.button.isCheckedChanged += [this, &settings] {
			settings.autopilot.levelChange = altitude.button.isChecked();
			settings.autopilot.scheduleWrite();
		};

		altitudeLayout.setFocusable(false);

		row += &altitudeLayout;
	}
}
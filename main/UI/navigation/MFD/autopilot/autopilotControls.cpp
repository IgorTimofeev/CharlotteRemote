#include "autopilotControls.h"
#include "../../../../rc.h"

namespace pizda {
	using namespace YOBA;

	AutopilotControls::AutopilotControls() {
		setHeight(52);

		auto& settings = RC::getInstance().getSettings();

		row.setSpacing(8);

		// FD
		FDButton.setSize(Size(26, 20));
		FDButton.setVerticalAlignment(Alignment::center);
		FDButton.setCheckMode(ButtonCheckMode::toggle);
		FDButton.setChecked(settings.interface.MFD.PFD.flightDirectors);

		FDButton.isCheckedChanged += [this] {
			auto& settings = RC::getInstance().getSettings();
			settings.interface.MFD.PFD.flightDirectors = FDButton.isChecked();
			settings.enqueueWrite();
		};
		
		row += &FDButton;

		// Speed
		speed.seven.setValue(settings.autopilot.speedKt);
		speed.button.setChecked(settings.autopilot.autoThrottle);

		speed.rotated += [this] {
			auto& settings = RC::getInstance().getSettings();
			settings.autopilot.speedKt = speed.seven.getValue();
			settings.enqueueWrite();
		};

		speed.button.isCheckedChanged += [this] {
			auto& settings = RC::getInstance().getSettings();
			settings.autopilot.autoThrottle = speed.button.isChecked();
			settings.enqueueWrite();
		};

		speedLayout.setFocusable(false);
		row += &speedLayout;

		// Heading
		heading.seven.setValue(settings.autopilot.headingDeg);
		heading.button.setChecked(settings.autopilot.headingHold);

		heading.rotated += [this] {
			auto& settings = RC::getInstance().getSettings();
			settings.autopilot.headingDeg = heading.seven.getValue();
			settings.enqueueWrite();
		};

		heading.button.isCheckedChanged += [this] {
			auto& settings = RC::getInstance().getSettings();
			settings.autopilot.headingHold = heading.button.isChecked();
			settings.enqueueWrite();
		};

		headingLayout.setFocusable(false);
		row += &headingLayout;

		// Altitude
		altitude.seven.setValue(settings.autopilot.altitudeFt);
		altitude.button.setChecked(settings.autopilot.levelChange);

		altitude.rotated += [this] {
			auto& settings = RC::getInstance().getSettings();
			settings.autopilot.altitudeFt = altitude.seven.getValue();
			settings.enqueueWrite();
		};

		altitude.button.isCheckedChanged += [this] {
			auto& settings = RC::getInstance().getSettings();
			settings.autopilot.levelChange = altitude.button.isChecked();
			settings.enqueueWrite();
		};

		altitudeLayout.setFocusable(false);

		row += &altitudeLayout;
	}
}
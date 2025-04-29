#include "autopilotControls.h"
#include "../../../../rc.h"

namespace pizda {
	using namespace YOBA;

	AutopilotControls::AutopilotControls() {
		setHeight(48);

		auto& settings = RC::getInstance().getSettings();

		row.setSpacing(8);

		// Speed
		speed.seven.setValue(settings.autopilot.speedKt);
		speed.button.setChecked(settings.autopilot.autoThrottle);

		speed.rotated += [this]() {
			auto& settings = RC::getInstance().getSettings();
			settings.autopilot.speedKt = speed.seven.getValue();
			RC::getInstance().getSettings().enqueueWrite();
		};

		speed.button.isCheckedChanged += [this]() {
			auto& settings = RC::getInstance().getSettings();
			settings.autopilot.autoThrottle = speed.button.isChecked();
			RC::getInstance().getSettings().enqueueWrite();
		};

		speedLayout.setFocusable(false);
		row += &speedLayout;

		// Heading
		heading.seven.setValue(settings.autopilot.headingDeg);
		heading.button.setChecked(settings.autopilot.headingHold);

		heading.rotated += [this]() {
			auto& settings = RC::getInstance().getSettings();
			settings.autopilot.headingDeg = heading.seven.getValue();
			RC::getInstance().getSettings().enqueueWrite();
		};

		heading.button.isCheckedChanged += [this]() {
			auto& settings = RC::getInstance().getSettings();
			settings.autopilot.headingHold = heading.button.isChecked();
			RC::getInstance().getSettings().enqueueWrite();
		};

		headingLayout.setFocusable(false);
		row += &headingLayout;

		// Altitude
		altitude.seven.setValue(settings.autopilot.altitudeFt);
		altitude.button.setChecked(settings.autopilot.levelChange);

		altitude.rotated += [this]() {
			auto& settings = RC::getInstance().getSettings();
			settings.autopilot.altitudeFt = altitude.seven.getValue();
			RC::getInstance().getSettings().enqueueWrite();
		};

		altitude.button.isCheckedChanged += [this]() {
			auto& settings = RC::getInstance().getSettings();
			settings.autopilot.levelChange = altitude.button.isChecked();
			RC::getInstance().getSettings().enqueueWrite();
		};

		altitudeLayout.setFocusable(false);

		row += &altitudeLayout;
	}
}
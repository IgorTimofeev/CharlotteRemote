#include "autopilotControls.h"
#include "../../../../../rc.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	AutopilotControls::AutopilotControls() {
		setHeight(48);

		auto& settings = RC::getInstance().getSettings();

		// Speed
		speed.seven.setValue(settings.autopilot.speedKt);
		speed.button.setChecked(settings.autopilot.autoThrottle);

		speed.rotated += [this]() {
			auto& settings = RC::getInstance().getSettings();
			settings.autopilot.speedKt = speed.seven.getValue();
			RC::getInstance().getSettings().enqueueWrite();
		};

		speed.switched += [this]() {
			auto& settings = RC::getInstance().getSettings();
			settings.autopilot.autoThrottle = speed.button.isChecked();
			RC::getInstance().getSettings().enqueueWrite();
		};

		row += &speedTitle;

		// Heading
		heading.seven.setValue(settings.autopilot.headingDeg);
		heading.button.setChecked(settings.autopilot.headingHold);

		heading.rotated += [this]() {
			auto& settings = RC::getInstance().getSettings();
			settings.autopilot.headingDeg = heading.seven.getValue();
			RC::getInstance().getSettings().enqueueWrite();
		};

		heading.switched += [this]() {
			auto& settings = RC::getInstance().getSettings();
			settings.autopilot.headingHold = heading.button.isChecked();
			RC::getInstance().getSettings().enqueueWrite();
		};

		row += &headingTitle;

		// Altitude
		altitude.seven.setValue(settings.autopilot.altitudeFt);
		altitude.button.setChecked(settings.autopilot.levelChange);

		altitude.rotated += [this]() {
			auto& settings = RC::getInstance().getSettings();
			settings.autopilot.altitudeFt = altitude.seven.getValue();
			RC::getInstance().getSettings().enqueueWrite();
		};

		altitude.switched += [this]() {
			auto& settings = RC::getInstance().getSettings();
			settings.autopilot.levelChange = altitude.button.isChecked();
			RC::getInstance().getSettings().enqueueWrite();
		};

		row += &altitudeTitle;

		row.setOrientation(Orientation::horizontal);
		row.setSpacing(10);
		row.setHorizontalAlignment(Alignment::center);
		row.setMargin(Margin(0, 2, 0, 0));
		*this += &row;
	}
}
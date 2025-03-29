#include "autopilotControls.h"
#include "../../../../../rc.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	AutopilotControls::AutopilotControls() {
		setHeight(48);

		row.setOrientation(Orientation::horizontal);
		row.setSpacing(10);

		auto& settings = RC::getInstance().getSettings();

		// Speed
		speed.addSettingsCallbacks(&settings.autopilot.speed, &settings.autopilot.autoThrottle);
		row += &headingTitle;

		// Heading
		heading.addSettingsCallbacks(&settings.autopilot.heading, &settings.autopilot.headingHold);
		row += &headingTitle;

		// Altitude
		heading.addSettingsCallbacks(&settings.autopilot.altitude, &settings.autopilot.levelChange);
		row += &altitudeTitle;

		*this += &row;
	}
}
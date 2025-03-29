#include "autopilotControls.h"
#include "../../../../../rc.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	AutopilotControls::AutopilotControls() {
		setHeight(48);

		row.setOrientation(Orientation::horizontal);
		row.setSpacing(10);

		speed.rotated += [this](bool positive, bool big) {
			auto& settings = RC::getInstance().getSettings();

			settings.autopilot.speed = addSaturating(settings.autopilot.speed, (big ? 10 : 1) * (positive ? 1 : -1));
			settings.enqueueWrite();

			speed.seven.setValue(settings.autopilot.speed);
			invalidate();
		};

		speed.button.isCheckedChanged += [this]() {
			auto& settings = RC::getInstance().getSettings();

			settings.autopilot.autoThrottle = speed.button.isChecked();
			settings.enqueueWrite();
		};

		row += &speedTitle;

		row += &headingTitle;

		row += &altitudeTitle;

		row += &pressureTitle;

		*this += &row;
	}
}
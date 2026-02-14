#include "baroToolbar.h"
#include "rc.h"

namespace pizda {
	BaroToolbar::BaroToolbar() {
		auto& rc = RC::getInstance();

		row += &pressure;
		row += &minimums;

		// Metric units
		metricUnits.setActive(rc.getSettings().personalization.MFD.PFD.metricUnits);

		metricUnits.onPressed = [this, &rc] {
			rc.getSettings().personalization.MFD.PFD.metricUnits = !rc.getSettings().personalization.MFD.PFD.metricUnits;
			rc.getSettings().personalization.scheduleWrite();

			metricUnits.setActive(rc.getSettings().personalization.MFD.PFD.metricUnits);

			rc.playFeedback();
		};

		row += &metricUnits;
	}
}

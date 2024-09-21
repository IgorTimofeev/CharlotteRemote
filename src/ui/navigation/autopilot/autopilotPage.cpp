#include "autopilotPage.h"
#include "../../rc_application.h"

namespace pizdanc {
	void AutopilotPage::begin() {
		auto& app = RCApplication::getInstance();

		columns.setAlignment(Alignment::Center);
		columns.setOrientation(Orientation::Horizontal);
		columns.setSpacing(20);

		columns += &spd;
		columns += &hdg;
		columns += &alt;

		*this += &columns;

		// Speed
		spd.seven.setValue((uint32_t) app.getLocalData().getAutopilotSpeed());

		spd.knob.addOnRotate([&](float delta) {
			auto newValue = constrain(app.getLocalData().getAutopilotSpeed() + delta, 0.0f, 999.0f);
			app.getLocalData().setAutopilotSpeed(newValue);
			spd.seven.setValue((uint32_t) newValue);
		});

		// Heading
		hdg.seven.setValue((uint32_t) app.getLocalData().getAutopilotHeading());

		hdg.knob.addOnRotate([&](float delta) {
			auto newValue = app.getLocalData().getAutopilotSpeed() + delta;

			if (newValue < 0)
				newValue += 360;
			else if (newValue >= 360)
				newValue -= 360;

			app.getLocalData().setAutopilotHeading(newValue);
		});

		// Altitude
		alt.seven.setValue((uint32_t) app.getLocalData().getAutopilotAltitude());

		alt.knob.addOnRotate([&](float delta) {
			app.getLocalData().setAutopilotAltitude(constrain(app.getLocalData().getAutopilotAltitude() + delta, 0.0f, 9999.0f));
		});
	}
}
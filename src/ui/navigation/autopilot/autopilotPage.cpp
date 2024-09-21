#include "autopilotPage.h"
#include "../../rc_application.h"

namespace pizdanc {
	void AutopilotPage::begin() {
		auto& app = RCApplication::getInstance();

		columns.setAlignment(Alignment::Center);
		columns.setOrientation(Orientation::Horizontal);
		columns.setSpacing(20);

		columns.addChild(&spd);
		columns.addChild(&hdg);
		columns.addChild(&alt);

		addChild(&columns);

		// Speed
		addIndicatorCallback(spd, app.getLocalData().getAutopilotSpeed(), 1, [&](float value) {
			app.getLocalData().setAutopilotSpeed(value);
		});

		// Heading
		addIndicatorCallback(hdg, app.getLocalData().getAutopilotHeading(), 1, [&](float value) {
			app.getLocalData().setAutopilotHeading(value);
		});

		// Altitude
		addIndicatorCallback(alt, app.getLocalData().getAutopilotAltitude(), 10, [&](float value) {
			app.getLocalData().setAutopilotAltitude(value);
		});
	}

	void AutopilotPage::addIndicatorCallback(AutopilotSelector& selector, float defaultValue, uint8_t incrementBy, const std::function<void(float)>& callback) {
		selector.sevenSegment.setValue(defaultValue == 0 ? SevenSegment::dashes : (uint32_t) defaultValue);

		selector.rotaryKnob.addOnRotate([&selector, incrementBy, callback](float delta) {
			auto oldValue = selector.sevenSegment.getValue();

			auto newValue =
				delta > 0
				? oldValue + incrementBy
				: (
					oldValue >= incrementBy
					? oldValue - incrementBy
					: 0
				);

			selector.sevenSegment.setValue(newValue);

			Serial.printf("Delta: %f, new angle: %f\n", delta, degrees(selector.rotaryKnob.getAngle()));

			callback((float) newValue);
		});
	}
}
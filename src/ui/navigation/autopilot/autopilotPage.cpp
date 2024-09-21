#include "autopilotPage.h"
#include "../../rc_application.h"

namespace pizdanc {
	void AutopilotPage::begin() {
		auto& app = RCApplication::getInstance();

		columns.setAlignment(Alignment::Center);
		columns.setOrientation(Orientation::Horizontal);
		columns.setSpacing(20);

		columns.addChild(&spd);
		columns.addChild(&alt);
		columns.addChild(&pressure);

		addChild(&columns);

		// Speed
		addIndicatorCallback(spd, app.getLocalData().getAutopilotSpeed(), [&](float value) {
			app.getLocalData().setAutopilotSpeed(value);
		});

		// Altitude
		addIndicatorCallback(alt, app.getLocalData().getAutopilotAltitude(), [&](float value) {
			app.getLocalData().setAutopilotAltitude(value);
		});

		// Pressure
		addIndicatorCallback(pressure, app.getLocalData().getAltimeterPressure(), [&](float value) {
			app.getLocalData().setAltimeterPressure(value);
		});
	}

	void AutopilotPage::addIndicatorCallback(AutopilotSelector& selector, float defaultValue, const std::function<void(float)>& callback) {
		selector.sevenSegment.setValue(defaultValue == 0 ? SevenSegment::dashes : (uint32_t) defaultValue);

		selector.rotaryKnob.addOnRotate([&selector, callback](float delta) {
			const auto inc = 1;

			auto oldValue = selector.sevenSegment.getValue();

			if (oldValue == SevenSegment::dashes)
				oldValue = 0;

			auto newValue =
				delta > 0
				? oldValue + inc
				: (
					oldValue >= inc
					? oldValue - inc
					: 0
				);

			selector.sevenSegment.setValue(newValue == 0 ? SevenSegment::dashes : newValue);

			Serial.printf("Delta: %f, new angle: %f\n", delta, degrees(selector.rotaryKnob.getAngle()));

			callback((float) newValue);
		});
	}
}
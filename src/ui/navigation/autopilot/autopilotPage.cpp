#include "autopilotPage.h"
#include "../../rc_application.h"

namespace pizdanc {
	AutopilotPage::AutopilotPage(RCApplication* application) : _application(application) {
		columns.setAlignment(Alignment::Center);
		columns.setOrientation(Orientation::Horizontal);
		columns.setSpacing(20);

		spd.sevenSegment.setValue(application->getLocalData().getAutopilotSpeed() == 0 ? SevenSegment::dashes : (uint32_t) application->getLocalData().getAutopilotSpeed());

		spd.rotaryKnob.addOnRotate([&](float delta) {
			const auto inc = 1;

			auto oldValue = spd.sevenSegment.getValue();

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

			spd.sevenSegment.setValue(newValue == 0 ? SevenSegment::dashes : newValue);

			auto &app = RCApplication::getInstance();

			app.getLocalData().setAutopilotSpeed((float) newValue);

			Serial.printf("Delta: %f, new angle: %f\n", delta, degrees(spd.rotaryKnob.getAngle()));
		});

		columns.addChild(&spd);

		columns.addChild(&alt);

		columns.addChild(&baro);

		addChild(&columns);
	}
}
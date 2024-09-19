#include "autopilotPage.h"
#include "../../rc_application.h"

namespace pizdanc {
	AutopilotPage::AutopilotPage() {
		columns.setAlignment(Alignment::Center);
		columns.setOrientation(Orientation::Horizontal);
		columns.setSpacing(20);

		spd.rotaryKnob.addOnRotate([&](float delta) {
			const auto inc = 1;

			auto newValue =
				delta > 0
				? spd.sevenSegment.getValue() + inc
				: (
					spd.sevenSegment.getValue() >= inc
					? spd.sevenSegment.getValue() - inc
					: 0

				);
			spd.sevenSegment.setValue(newValue);

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
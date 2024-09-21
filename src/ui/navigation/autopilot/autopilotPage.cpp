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

		auto govnoedstvo = [](
			AutopilotSelector& selector,
			const std::function<float()>& valueGetter,
			const std::function<void(KnobRotateEvent&)>& valueSetter
		) {
			selector.seven.setValue((uint32_t) valueGetter());

			selector.knob.getOnRotate().add([&selector, valueGetter, valueSetter](KnobRotateEvent& event) {
				valueSetter(event);

				selector.seven.setValue((uint32_t) valueGetter());
			});
		};

		// Speed
		govnoedstvo(
			spd,
			[&]() {
				return app.getLocalData().getAutopilotSpeed();
			},
			[&](KnobRotateEvent& event) {
				app.getLocalData().setAutopilotSpeed(clamp(app.getLocalData().getAutopilotSpeed() + (event.getDeltaAngle() > 0 ? 1.0f : -1.0f), 0.0f, 999.0f));
			}
		);

		// Heading
		govnoedstvo(
			hdg,
			[&]() {
				return app.getLocalData().getAutopilotHeading();
			},
			[&](KnobRotateEvent& event) {
				auto newValue = (float) degrees(event.getNewAngle());

				if (newValue < 0) {
					newValue += 360;
				}
				else if (newValue >= 360) {
					newValue -= 360;
				}

				app.getLocalData().setAutopilotHeading(newValue);
			}
		);

		// Altitude
		govnoedstvo(
			alt,
			[&]() {
				return app.getLocalData().getAutopilotAltitude();
			},
			[&](KnobRotateEvent& event) {
				app.getLocalData().setAutopilotAltitude(clamp(app.getLocalData().getAutopilotAltitude() + (event.getDeltaAngle() > 0 ? 10.0f : -10.0f), 0.0f, 9999.0f));
			}
		);
	}
}
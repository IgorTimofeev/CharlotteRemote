#include "autopilotPage.h"
#include "../../rc_application.h"

namespace pizdanc {
	void AutopilotPage::setup() {
		auto& app = RCApplication::getInstance();

		_columns.setAlignment(Alignment::Center);
		_columns.setOrientation(Orientation::Horizontal);
		_columns.setSpacing(20);

		_columns += &_spd;
		_columns += &_hdg;
		_columns += &_alt;

		*this += &_columns;

		auto govnoedstvo = [](
			AutopilotSelector& selector,
			const std::function<float()>& valueGetter,
			const std::function<void(KnobRotateEvent&)>& valueSetter
		) {
			selector.seven.setValue((uint32_t) valueGetter());

			selector.knob.getOnRotate() += [&selector, valueGetter, valueSetter](KnobRotateEvent& event) {
				valueSetter(event);

				selector.seven.setValue((uint32_t) valueGetter());
			};
		};

		// Speed
		govnoedstvo(
			_spd,
			[&]() {
				return app.getLocalData().getAutopilotSpeed();
			},
			[&](KnobRotateEvent& event) {
				app.getLocalData().setAutopilotSpeed(clamp(app.getLocalData().getAutopilotSpeed() + (event.getDeltaAngle() > 0 ? 1.0f : -1.0f), 0.0f, 999.0f));
			}
		);

		// Heading
		govnoedstvo(
			_hdg,
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
			_alt,
			[&]() {
				return app.getLocalData().getAutopilotAltitude();
			},
			[&](KnobRotateEvent& event) {
				app.getLocalData().setAutopilotAltitude(clamp(app.getLocalData().getAutopilotAltitude() + (event.getDeltaAngle() > 0 ? 10.0f : -10.0f), 0.0f, 9999.0f));
			}
		);
	}
}
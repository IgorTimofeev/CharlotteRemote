#include "autopilotPage.h"
#include "../../../rc.h"

namespace pizdanc {
	void AutopilotPage::setup() {
		auto& rc = RC::getInstance();

		_columns.setAlignment(Alignment::center);
		_columns.setOrientation(Orientation::horizontal);
		_columns.setSpacing(20);

		_columns += &_spd;
		_columns += &_hdg;
		_columns += &_alt;

		*this += &_columns;

		auto govnoedstvo = [](
			AutopilotSelector& selector,
			const std::function<float()>& valueGetter,
			const std::function<void(float, float)>& valueSetter
		) {
			selector.seven.setValue((uint32_t) valueGetter());

			selector.knob.rotated += [&selector, valueGetter, valueSetter](float oldAngle, float newAngle) {
				valueSetter(oldAngle, newAngle);

				selector.seven.setValue((uint32_t) valueGetter());
			};
		};

		// Speed
		govnoedstvo(
			_spd,
			[&]() {
				return rc.getLocalData().getAutopilotSpeed();
			},
			[&](float oldAngle, float newAngle) {
				rc.getLocalData().setAutopilotSpeed(clamp(rc.getLocalData().getAutopilotSpeed() + (newAngle - oldAngle > 0 ? 1.0f : -1.0f), 0.0f, 999.0f));
			}
		);

		// Heading
		govnoedstvo(
			_hdg,
			[&]() {
				return rc.getLocalData().getAutopilotHeading();
			},
			[&](float oldAngle, float newAngle) {
				auto newValue = (float) toDegrees(newAngle);

				if (newValue < 0) {
					newValue += 360;
				}
				else if (newValue >= 360) {
					newValue -= 360;
				}

				rc.getLocalData().setAutopilotHeading(newValue);
			}
		);

		// Altitude
		govnoedstvo(
			_alt,
			[&]() {
				return rc.getLocalData().getAutopilotAltitude();
			},
			[&](float oldAngle, float newAngle) {
				rc.getLocalData().setAutopilotAltitude(clamp(rc.getLocalData().getAutopilotAltitude() + (newAngle - oldAngle > 0 ? 10.0f : -10.0f), 0.0f, 9999.0f));
			}
		);
	}
}
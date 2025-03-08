#include "autopilotPage.h"
#include "../../../rc.h"

namespace pizda {
	AutopilotPage::AutopilotPage() {
		title.setText(L"Autopilot");

		rows += &_spd;
		rows += &_hdg;
		rows += &_alt;

		auto govnoedstvo = [](
			AutopilotSelector& selector,
			uint16_t* settingsValue,
			const std::function<uint16_t(float, float)>& valueConverter
		) {
			selector.seven.setValue(*settingsValue);

			selector.knob.rotated += [&selector, settingsValue, valueConverter](float oldAngle, float newAngle) {
				*settingsValue = valueConverter(oldAngle, newAngle);
				RC::getInstance().getSettings().enqueueWrite();

				selector.seven.setValue(*settingsValue);
			};
		};

		// Speed
		govnoedstvo(
			_spd,
			&RC::getInstance().getSettings().autopilot.speed,
			[](float oldAngle, float newAngle) {
				return yoba::addSaturating(RC::getInstance().getSettings().autopilot.speed, newAngle > oldAngle ? 1 : -1);
			}
		);

		// Heading
		govnoedstvo(
			_hdg,
			&RC::getInstance().getSettings().autopilot.heading,
			[](float oldAngle, float newAngle) {
				auto newValue = (float) yoba::toDegrees(newAngle);

				if (newValue < 0) {
					newValue += 360;
				}
				else if (newValue >= 360) {
					newValue -= 360;
				}

				return (uint16_t) newValue;
			}
		);

		// Altitude
		govnoedstvo(
			_alt,
			&RC::getInstance().getSettings().autopilot.altitude,
			[](float oldAngle, float newAngle) {
				const auto deltaAngle = newAngle - oldAngle;
				const uint8_t factor = std::abs(deltaAngle) > 5 ? 10 : 1;

				return yoba::addSaturating(RC::getInstance().getSettings().autopilot.altitude, deltaAngle > 0 ? factor : -factor);
			}
		);
	}
}
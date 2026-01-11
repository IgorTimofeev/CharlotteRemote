#pragma once

#include "types/settings/controlSettings.h"
#include "types/settings/axisSettings.h"
#include "types/settings/autopilotSettings.h"
#include "types/settings/personalizationSettings.h"
#include "types/settings/navigationSettings.h"
#include "types/settings/motorSettings.h"

namespace pizda {
	class Settings {
		public:
			ControlSettings controls {};
			AxisSettings axis {};
			AutopilotSettings autopilot {};
			PersonalizationSettings personalization {};
			NavigationSettings navigation {};
			MotorSettings motors {};

			void readAll() {
				controls.read();
				axis.read();
				autopilot.read();
				personalization.read();
				navigation.read();
				motors.read();
			}
	};
}

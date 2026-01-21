#pragma once

#include "types/settings/controlSettings.h"
#include "types/settings/axesSettings.h"
#include "types/settings/autopilotSettings.h"
#include "types/settings/personalizationSettings.h"
#include "types/settings/navigationSettings.h"
#include "types/settings/motorsSettings.h"

namespace pizda {
	class Settings {
		public:
			ControlSettings controls {};
			AxesSettings axes {};
			AutopilotSettings autopilot {};
			PersonalizationSettings personalization {};
			NavigationSettings navigation {};
			MotorsSettings motors {};

			void readAll() {
				controls.read();
				axes.read();
				autopilot.read();
				personalization.read();
				navigation.read();
				motors.read();
			}
	};
}

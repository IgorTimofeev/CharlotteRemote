#pragma once

#include "types/settings/settingsControls.h"
#include "types/settings/settingsAxis.h"
#include "types/settings/settingsAutopilot.h"
#include "types/settings/settingsPersonalization.h"
#include "types/settings/settingsNavigation.h"

namespace pizda {
	class Settings {
		public:
			SettingsControls controls {};
			SettingsAxis axis {};
			SettingsAutopilot autopilot {};
			SettingsPersonalization personalization {};
			SettingsNavigation navigation {};

			void readAll() {
				controls.read();
				axis.read();
				autopilot.read();
				personalization.read();
				navigation.read();
			}
	};
}

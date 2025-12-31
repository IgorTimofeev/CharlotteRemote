#pragma once

#include <cstdint>
#include <esp_timer.h>
#include <esp_log.h>
#include <memory>
#include <cstring>

#include "settingsControls.h"
#include "settingsAxis.h"
#include "settingsAutopilot.h"
#include "settingsPersonalization.h"
#include "settingsNavigation.h"

namespace pizda {
	using namespace YOBA;
	
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

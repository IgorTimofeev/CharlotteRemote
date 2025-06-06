#pragma once

#include <cstdint>
#include <esp_timer.h>
#include <esp_log.h>
#include <memory>
#include <cstring>

#include "settingsControls.h"
#include "settingsAxis.h"
#include "settingsAutopilot.h"
#include "settingsInterface.h"
#include "settingsNavigation.h"
#include "settingsWiFi.h"

namespace pizda {
	class Settings {
		public:
			SettingsControls controls {};
			SettingsAxis axis {};
			SettingsAutopilot autopilot {};
			SettingsInterface interface {};
			SettingsNavigation navigation {};
			SettingsWiFi WiFi {};

			void readAll() {
				controls.read();
				axis.read();
				autopilot.read();
				interface.read();
				navigation.read();
				WiFi.read();
			}
	};
}

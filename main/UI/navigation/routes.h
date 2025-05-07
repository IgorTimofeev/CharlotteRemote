#pragma once

#include "route.h"

#include "MFD/MFDPage.h"

#include "settings/MFD/MFDSettingsPage.h"
#include "settings/personalization/personalizationSettingsPage.h"
#include "settings/axis/axisSettingsPage.h"
#include "settings/WiFi/WiFiSettingsPage.h"
#include "settings/developer/developerSettingsPage.h"

namespace pizda {
	class Routes {
		public:
			constexpr static const ElementRoute<MFDPage> MFD = {};

			constexpr static const ElementRoute<MFDSettingsPage> settingsMFD = {};
			constexpr static const ElementRoute<PersonalizationSettingsPage> settingsPersonalization = {};
			constexpr static const ElementRoute<WiFiSettingsPage> settingsWiFi = {};
			constexpr static const ElementRoute<AxisSettingsPage> settingsAxis = {};
			constexpr static const ElementRoute<DeveloperSettingsPage> settingsDeveloper = {};
	};
}
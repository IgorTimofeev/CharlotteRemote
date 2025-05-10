#pragma once

#include "route.h"

#include "MFD/MFDPage.h"

#include "nav/flightPlan/flightPlanPage.h"

#include "settings/MFD/MFDSettingsPage.h"
#include "settings/personalization/personalizationSettingsPage.h"
#include "settings/axis/axisSettingsPage.h"
#include "settings/WiFi/WiFiSettingsPage.h"
#include "settings/developer/developerSettingsPage.h"

namespace pizda {
	class Routes {
		public:
			constexpr static ElementRoute<MFDPage> MFD = {};

			constexpr static ElementRoute<FlightPlanPage> navFlightPlan = {};

			constexpr static ElementRoute<MFDSettingsPage> settingsMFD = {};
			constexpr static ElementRoute<PersonalizationSettingsPage> settingsPersonalization = {};
			constexpr static ElementRoute<WiFiSettingsPage> settingsWiFi = {};
			constexpr static ElementRoute<AxisSettingsPage> settingsAxis = {};
			constexpr static ElementRoute<DeveloperSettingsPage> settingsDeveloper = {};
	};
}
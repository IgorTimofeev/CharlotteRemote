#pragma once

#include "route.h"

#include "MFD/MFDPage.h"

#include <UI/navigation/nav/flightPlan/flightPlanPage.h>
#include <UI/navigation/nav/waypoints/waypointsPage.h>

#include <UI/navigation/settings/MFD/MFDSettingsPage.h>
#include <UI/navigation/settings/personalization/personalizationSettingsPage.h>
#include <UI/navigation/settings/axis/axisSettingsPage.h>
#include <UI/navigation/settings/developer/developerSettingsPage.h>

namespace pizda {
	class Routes {
		public:
			constexpr static ElementRoute<MFDPage> MFD {};

			constexpr static ElementRoute<FlightPlanPage> navFlightPlan {};
			constexpr static ElementRoute<WaypointsPage> navWaypoints {};

			constexpr static ElementRoute<MFDSettingsPage> settingsMFD {};
			constexpr static ElementRoute<PersonalizationSettingsPage> settingsPersonalization {};
			constexpr static ElementRoute<AxisSettingsPage> settingsAxis {};
			constexpr static ElementRoute<DeveloperSettingsPage> settingsDeveloper {};
	};
}
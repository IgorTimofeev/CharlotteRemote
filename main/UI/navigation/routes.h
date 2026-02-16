#pragma once

#include "route.h"

#include "MFD/MFDPage.h"

#include "UI/navigation/NAV/flightPlan/flightPlanPage.h"
#include "UI/navigation/NAV/database/databasePage.h"

#include "UI/navigation/settings/personalization/personalizationSettingsPage.h"
#include "UI/navigation/settings/ADIRS/ADIRSSettingsPage.h"
#include "UI/navigation/settings/spectrumScan/spectrumScanPage.h"
#include "UI/navigation/settings/autopilot/autopilotSettingsPage.h"
#include "UI/navigation/settings/motors/motorsSettingsPage.h"
#include "UI/navigation/settings/axis/axesSettingsPage.h"
#include "UI/navigation/settings/transceiver/transceiverSettingsPage.h"

namespace pizda {
	class Routes {
		public:
			constexpr static ElementRoute<MFDPage> MFD {};

			constexpr static ElementRoute<FlightPlanPage> navFlightPlan {};
			constexpr static ElementRoute<DatabasePage> navWaypoints {};

			constexpr static ElementRoute<PersonalizationSettingsPage> settingsPersonalization {};
			constexpr static ElementRoute<ADIRSSettingsPage> settingsADIRS {};
			constexpr static ElementRoute<SpectrumScanPage> settingsSpectrumScan {};
			constexpr static ElementRoute<AutopilotSettingsPage> settingsAutopilot {};
			constexpr static ElementRoute<MotorsSettingsPage> settingsMotors {};
			constexpr static ElementRoute<AxesSettingsPage> settingsAxes {};
			constexpr static ElementRoute<TransceiverSettingsPage> settingsTransceiver {};
	};
}
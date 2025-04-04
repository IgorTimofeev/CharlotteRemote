#pragma once

#include "route.h"

#include "main/MFD/MFDPage.h"
#include "main/ND/ndPage.h"
#include "settings/axis/axisPage.h"
#include "settings/WiFiPage.h"
#include "settings/updatesPage.h"
#include "developer/speakerPage.h"
#include "developer/UITestPage.h"

namespace pizda {
	class Routes {
		public:
			constexpr static const ElementRoute<MFDPage> MFD = {};
			constexpr static const ElementRoute<NDPage> ND = {};

			// Settings
			constexpr static const ElementRoute<WiFiPage> settingsWiFi = {};
			constexpr static const ElementRoute<AxisPage> settingsAxis = {};
			constexpr static const ElementRoute<UpdatesPage> settingsUpdates = {};

			// Developer
			constexpr static const ElementRoute<SpeakerPage> developerSpeaker = {};
			constexpr static const ElementRoute<UITestPage> developerUITest = {};
	};
}

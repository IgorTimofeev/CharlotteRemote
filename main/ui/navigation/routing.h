#pragma once

#include <functional>

#include "../../../components/yoba/src/main.h"
#include "../../../components/yoba/src/ui.h"

#include "page.h"

#include "main/pfd/pfdPage.h"
#include "main/nd/ndPage.h"
#include "main/autopilot/autopilotPage.h"

#include "settings/axis/axisPage.h"
#include "settings/WiFiPage.h"
#include "settings/updatesPage.h"
#include "developer/speakerPage.h"
#include "developer/UITestPage.h"

namespace pizda {
	class Route {
		public:
			virtual Page* newPage() const = 0;
	};

	template<typename TPage>
	class PageRoute : public Route {
		public:
			Page* newPage() const override;
	};

	template<typename TPage>
	Page* PageRoute<TPage>::newPage() const {
		return new TPage();
	}

	class Routes {
		public:
			// Main
			constexpr static const PageRoute<PFDPage> PFD = {};
			constexpr static const PageRoute<NDPage> ND = {};
			constexpr static const PageRoute<AutopilotPage> autopilot = {};

			// Settings
			constexpr static const PageRoute<WiFiPage> settingsWiFi = {};
			constexpr static const PageRoute<AxisPage> settingsAxis = {};
			constexpr static const PageRoute<UpdatesPage> settingsUpdates = {};

			// Developer
			constexpr static const PageRoute<SpeakerPage> developerSpeaker = {};
			constexpr static const PageRoute<UITestPage> developerUITest = {};
	};
}
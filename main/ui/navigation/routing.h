#pragma once

#include <functional>
#include "page.h"
#include "pfd/pfdPage.h"
#include "nd/ndPage.h"
#include "autopilot/autopilotPage.h"
#include "debug/debugPage.h"
#include "axis/axisPage.h"
#include "wifi/WiFiPage.h"

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
			constexpr static const PageRoute<WiFiPage> WiFi = {};
			constexpr static const PageRoute<AxisPage> axis = {};
			constexpr static const PageRoute<DebugPage> debug = {};
	};
}
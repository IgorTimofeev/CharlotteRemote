#pragma once

#include "route.h"

#include "main/MFD/MFDPage.h"
#include "settings/axis/axisPage.h"
#include "settings/WiFiPage.h"
#include "developer/UITestPage.h"

namespace pizda {
	class Routes {
		public:
			constexpr static const ElementRoute<MFDPage> MFD = {};

			// Settings
			constexpr static const ElementRoute<WiFiPage> settingsWiFi = {};
			constexpr static const ElementRoute<AxisPage> settingsAxis = {};
			constexpr static const ElementRoute<UITestPage> developerUITest = {};
	};
}

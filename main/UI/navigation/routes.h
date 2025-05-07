#pragma once

#include "route.h"

#include "menu/MFDMenuSection.h"
#include "MFD/MFDPage.h"
#include "axis/axisPage.h"
#include "WiFi/WiFiPage.h"
#include "personalization/personalizationPage.h"
#include "developer/UITestPage.h"

namespace pizda {
	class Routes {
		public:
			constexpr static const ElementRoute<MFDPage> MFD = {};
			constexpr static const ElementRoute<MFDMenuSection> MFDMenuUpperSection = {};

			constexpr static const ElementRoute<PersonalizationPage> personalization = {};
			constexpr static const ElementRoute<WiFiPage> WiFi = {};
			constexpr static const ElementRoute<AxisPage> axis = {};
			constexpr static const ElementRoute<UITestPage> UITest = {};
	};
}
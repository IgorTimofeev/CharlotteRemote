#pragma once

#include "route.h"

#include "MFD/MFDPage.h"
#include "settings/axis/axisPage.h"
#include "developer/UITestPage.h"

namespace pizda {
	class Routes {
		public:
			constexpr static const ElementRoute<MFDPage> MFD = {};
			constexpr static const ElementRoute<AxisPage> settingsAxis = {};
			constexpr static const ElementRoute<UITestPage> UITest = {};
	};
}
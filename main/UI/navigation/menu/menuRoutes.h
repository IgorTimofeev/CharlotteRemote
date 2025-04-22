#pragma once

#include "../route.h"

#include "mainMenuView.h"
#include "MFDMenuView.h"

namespace pizda {
	class MenuRoutes {
		public:
			constexpr static const ElementRoute<MainMenuView> main = {};
			constexpr static const ElementRoute<MFDMenuView> MFD = {};
	};
}

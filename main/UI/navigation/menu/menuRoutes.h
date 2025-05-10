#pragma once

#include "../MFD/MFDMenuView.h"
#include "../nav/navMenuView.h"
#include "../settings/settingsMenuView.h"

namespace pizda {
	class MenuRoutes {
		public:
			constexpr static ElementRoute<MFDMenuView> MFD = {};
			constexpr static ElementRoute<NavMenuView> nav = {};
			constexpr static ElementRoute<SettingsMenuView> settings = {};
	};
}
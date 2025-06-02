#pragma once

#include <UI/navigation/MFD/MFDMenuView.h>
#include <UI/navigation/nav/navMenuView.h>
#include <UI/navigation/settings/settingsMenuView.h>

namespace pizda {
	class MenuRoutes {
		public:
			constexpr static ElementRoute<MFDMenuView> MFD {};
			constexpr static ElementRoute<NavMenuView> nav {};
			constexpr static ElementRoute<SettingsMenuView> settings {};
	};
}
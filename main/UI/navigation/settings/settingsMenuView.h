#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include <UI/navigation/menu/pagesMenuView.h>
#include <UI/navigation/menu/pageMenuViewButton.h>
#include <UI/navigation/routes.h>
#include <resources/images.h>

namespace pizda {
	using namespace YOBA;

	class SettingsMenuView : public PagesMenuView {
		public:
			explicit SettingsMenuView();

			void setup() override;

		private:
			static const Route* _lastRoute;

			std::array<PageMenuViewButton, 5> _pageButtons {
				PageMenuViewButton(&resources::Images::menuIconMFD, L"MFD", &Routes::settingsMFD),
				PageMenuViewButton(&resources::Images::menuIconPersonalization, L"Prsnl", &Routes::settingsPersonalization),
				PageMenuViewButton(&resources::Images::menuIconAxis, L"Axis", &Routes::settingsAxis),
				PageMenuViewButton(&resources::Images::menuIconWiFi, L"Wi-Fi", &Routes::settingsWiFi),
				PageMenuViewButton(&resources::Images::menuIconDev, L"Dev", &Routes::settingsDeveloper),
			};

			MenuViewButton _powerButton = { &resources::Images::menuIconPower, L"Reboot" };
	};
}
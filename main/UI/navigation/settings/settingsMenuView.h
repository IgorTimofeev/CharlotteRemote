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

			std::array<PageMenuViewButton, 4> _pageButtons {
				PageMenuViewButton(&resources::images::menuIconMFD, L"MFD", &Routes::settingsMFD),
				PageMenuViewButton(&resources::images::menuIconPersonalization, L"Prsnl", &Routes::settingsPersonalization),
				PageMenuViewButton(&resources::images::menuIconAxis, L"Axis", &Routes::settingsAxis),
				PageMenuViewButton(&resources::images::menuIconDev, L"Dev", &Routes::settingsDeveloper)
			};

			MenuViewButton _powerButton = { &resources::images::menuIconPower, L"Reboot" };
	};
}
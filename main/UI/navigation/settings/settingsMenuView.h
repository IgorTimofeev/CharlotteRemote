#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "../menu/pagesMenuView.h"
#include "../menu/pageMenuViewButton.h"
#include "../routes.h"
#include "../../../resources/images.h"

namespace pizda {
	using namespace YOBA;

	class SettingsMenuView : public PagesMenuView {
		public:
			explicit SettingsMenuView();

			void setup() override;

		private:
			static const Route* _lastRoute;

			PageMenuViewButton _pageButtons[5] {
				{ &resources::Images::menuIconMFD, L"MFD", &Routes::settingsMFD },
				{ &resources::Images::menuIconPersonalization, L"Prsnl", &Routes::settingsPersonalization },
				{ &resources::Images::menuIconAxis, L"Axis", &Routes::settingsAxis },
				{ &resources::Images::menuIconWiFi, L"Wi-Fi", &Routes::settingsWiFi },
				{ &resources::Images::menuIconDev, L"Dev", &Routes::settingsDeveloper },
			};

			MenuViewButton _powerButton = { &resources::Images::menuIconPower, L"Reboot" };
	};
}
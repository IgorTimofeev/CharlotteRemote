#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "menuView.h"
#include "pageMenuViewButton.h"
#include "../routes.h"
#include "../../../resources/images.h"

namespace pizda {
	using namespace YOBA;

	class PagesMenuView : public MenuView {
		public:
			explicit PagesMenuView();

			const Route* getRoute() override;
			void setRoute(const Route* route);

		private:
			static const Route* _route;

			PageMenuViewButton
				_MFDButton = { &resources::Images::menuIconMFD, L"MFD", &Routes::settingsMFD },
				_personalizationButton = { &resources::Images::menuIconPersonalization, L"Person.", &Routes::settingsPersonalization },
				_axisButton = { &resources::Images::menuIconAxis, L"Axis", &Routes::settingsAxis },
				_WiFiButton = { &resources::Images::menuIconWiFi, L"Wi-Fi", &Routes::settingsWiFi },
				_UITestButton = { &resources::Images::menuIconDev, L"Dev", &Routes::settingsDeveloper };

			MenuViewButton _powerButton = { &resources::Images::menuIconPower, L"Reboot" };

			PageMenuViewButton* _buttons[5] = {
				&_MFDButton,
				&_personalizationButton,
				&_axisButton,
				&_WiFiButton,
				&_UITestButton,
			};
	};
}
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
				_MFDButton = PageMenuViewButton(&resources::Images::menuIconMFD, L"MFD", &Routes::settingsMFD),
				_personalizationButton = PageMenuViewButton(&resources::Images::menuIconPersonalization, L"Person.", &Routes::settingsPersonalization),
				_axisButton = PageMenuViewButton(&resources::Images::menuIconAxis, L"Axis", &Routes::settingsAxis),
				_WiFiButton = PageMenuViewButton(&resources::Images::menuIconWiFi, L"Wi-Fi", &Routes::settingsWiFi),
				_UITestButton = PageMenuViewButton(&resources::Images::menuIconDev, L"Dev", &Routes::settingsDeveloper);

			PageMenuViewButton* _buttons[5] = {
				&_MFDButton,
				&_personalizationButton,
				&_axisButton,
				&_WiFiButton,
				&_UITestButton,
			};
	};
}
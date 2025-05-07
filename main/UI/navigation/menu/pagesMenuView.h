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

			void setRoute(const Route* route);

		private:
			PageMenuViewButton
				_MFDButton = PageMenuViewButton(&resources::Images::menuIconMFD, L"MFD", &Routes::MFD),
				_personalizationButton = PageMenuViewButton(&resources::Images::menuIconPersonalization, L"Pers.", &Routes::personalization),
				_axisButton = PageMenuViewButton(&resources::Images::menuIconAxis, L"Axis", &Routes::axis),
				_WiFiButton = PageMenuViewButton(&resources::Images::menuIconWiFi, L"Wi-Fi", &Routes::WiFi),
				_UITestButton = PageMenuViewButton(&resources::Images::menuIconDev, L"Test", &Routes::UITest);

			PageMenuViewButton* _buttons[5] {
				&_MFDButton,
				&_personalizationButton,
				&_axisButton,
				&_WiFiButton,
				&_UITestButton,
			};
	};
}
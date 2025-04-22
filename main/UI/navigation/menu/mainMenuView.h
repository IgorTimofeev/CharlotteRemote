#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "menuView.h"
#include "menuButton.h"
#include "../routes.h"
#include "../../../resources/images.h"

namespace pizda {
	using namespace YOBA;

	class RouteMenuButton : public MenuButton {
		public:
			RouteMenuButton(const Image* image, const std::wstring_view&, const Route* route);

		protected:
			void onClick() override;

		private:
			const Route* _route;
	};

	class MainMenuView : public MenuView {
		public:
			explicit MainMenuView();

			MenuButton MFDButton = MenuButton(&resources::Images::menuIconMFD, L"MFD");

			RouteMenuButton
				axisButton = RouteMenuButton(&resources::Images::menuIconAxis, L"Axis", &Routes::settingsAxis),
				WiFiButton = RouteMenuButton(&resources::Images::menuIconWiFi, L"Wi-Fi", &Routes::settingsWiFi),
				UITestButton = RouteMenuButton(&resources::Images::menuIconDev, L"Test", &Routes::UITest);

			RouteMenuButton* routeButtons[3] {
				&axisButton,
				&WiFiButton,
				&UITestButton,
			};

			void checkButton(MenuButton* button);
	};
}
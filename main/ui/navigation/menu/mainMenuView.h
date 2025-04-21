#pragma once

#include "../../../../components/yoba/src/main.h"
#include "../../../../components/yoba/src/ui.h"

#include "menuView.h"
#include "menuButton.h"
#include "../routes.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class RouteMenuButton : public MenuButton {
		public:
			RouteMenuButton(const std::wstring_view&, const Route* route);

		protected:
			void onClick() override;

		private:
			const Route* _route;
	};

	class MainMenuView : public MenuView {
		public:
			explicit MainMenuView();

			MenuButton MFDButton = MenuButton(L"MFD");

			RouteMenuButton
				axisButton = RouteMenuButton(L"Axis", &Routes::settingsAxis),
				WiFiButton = RouteMenuButton(L"Wi-Fi", &Routes::settingsWiFi),
				UITestButton = RouteMenuButton(L"Test", &Routes::UITest);

			RouteMenuButton* routeButtons[3] {
				&axisButton,
				&WiFiButton,
				&UITestButton,
			};

			void checkButton(MenuButton* button);
	};
}
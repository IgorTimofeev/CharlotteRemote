#pragma once

#include "../../../../components/yoba/src/main.h"
#include "../../../../components/yoba/src/ui.h"

#include "menuView.h"
#include "menuButton.h"
#include "../routes.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class MainMenuView : public MenuView {
		public:
			explicit MainMenuView();

			RouteMenuButton
				MFDButton = RouteMenuButton(L"MFD", &Routes::MFD),
				axisButton = RouteMenuButton(L"Axis", &Routes::settingsAxis),
				WiFiButton = RouteMenuButton(L"Wi-Fi", &Routes::settingsWiFi),
				developerUITestButton = RouteMenuButton(L"Test", &Routes::UITest);
	};
}
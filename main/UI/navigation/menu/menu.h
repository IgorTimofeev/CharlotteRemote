#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "../../theme.h"
#include "../../../settings.h"

#include "menuSection.h"
#include "menuButton.h"
#include "routeMenuButton.h"
#include "../routes.h"
#include "../../../resources/images.h"
#include "MFDMenuSection.h"

namespace pizda {
	using namespace YOBA;

	class MenuOverlayBackground : public DitheredRectangle {
		public:
			MenuOverlayBackground();

		protected:
			void onEvent(Event* event) override;
	};

	class Menu : public RelativeStackLayout {
		public:
			explicit Menu();
			~Menu();

			void setRoute(const Route* route);

		private:
			MenuOverlayBackground _overlayBackground = {};

			Layout _slideLayout = {};
			Rectangle _slideBackground = {};

			StackLayout _rows {};

			MenuSection _lowerSection {};

			const Route* _upperSectionRoute = nullptr;
			Element* _upperSectionElement = nullptr;

			RouteMenuButton
				_MFDButton = RouteMenuButton(&resources::Images::menuIconMFD, L"MFD", &Routes::MFD, &Routes::MFDMenuUpperSection),
				_personalizationButton = RouteMenuButton(&resources::Images::menuIconPersonalization, L"Pers.", &Routes::personalization),
				_axisButton = RouteMenuButton(&resources::Images::menuIconAxis, L"Axis", &Routes::axis),
				_WiFiButton = RouteMenuButton(&resources::Images::menuIconWiFi, L"Wi-Fi", &Routes::WiFi),
				_UITestButton = RouteMenuButton(&resources::Images::menuIconDev, L"Test", &Routes::UITest);

			RouteMenuButton* _routeButtons[5] {
				&_MFDButton,
				&_personalizationButton,
				&_axisButton,
				&_WiFiButton,
				&_UITestButton,
			};

			void removeUpperSectionRoute();
			void addUpperSectionRoute(const Route* route);
	};
}
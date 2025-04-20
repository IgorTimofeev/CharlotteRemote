#pragma once

#include "../../../components/yoba/src/main.h"
#include "../../../components/yoba/src/ui.h"
#include "../theme.h"
#include "../../settings.h"

#include "menuButton.h"
#include "routes.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class MenuOverlayBackground : public DitheredRectangle {
		public:
			MenuOverlayBackground();

		protected:
			void onEvent(Event* event) override;
	};

	class Menu : public RelativeStackLayout {
		public:
			explicit Menu();

			void updateRouteButtonsSelection();
			void updateMFDModeButtonsSelection();

		private:
			MenuOverlayBackground _overlayBackground = {};

			Layout _slideLayout = {};
			Rectangle _slideBackground = {};
			StackLayout _slideRows = {};

			WrapLayout
				_mainWL = {},
				_settingsWL = {};

			Text
				_MFDTitle = Text(L"MFD options"),
				_settingsTitle = Text(L"Navigation");

			RouteMenuButton
				_mainMFDButton = RouteMenuButton(L"MFD", &Routes::MFD),
				_settingsAxisButton = RouteMenuButton(L"Axis", &Routes::settingsAxis),
				_settingsWiFiButton = RouteMenuButton(L"Wi-Fi", &Routes::settingsWiFi),
				_developerUITestButton = RouteMenuButton(L"Test", &Routes::developerUITest);

			OptionMenuButton
				_mainNDButton = OptionMenuButton(L"N/D");

			MFDModeMenuButton
				_mainMainButton = MFDModeMenuButton(L"Main", SettingsInterfaceMFDInstrumentsMode::main),
				_mainAutopilotButton = MFDModeMenuButton(L"A/P", SettingsInterfaceMFDInstrumentsMode::autopilot),
				_mainPressureButton = MFDModeMenuButton(L"Baro", SettingsInterfaceMFDInstrumentsMode::pressure);

			MFDModeMenuButton* _MFDInstrumentsModeButtons[3] {
				&_mainMainButton,
				&_mainAutopilotButton,
				&_mainPressureButton
			};

			RouteMenuButton* _routeButtons[4] {
				&_mainMFDButton,
				&_settingsAxisButton,
				&_settingsWiFiButton,
				&_developerUITestButton,
			};

			void addTitle(Text* text);
			void addRouteButton(WrapLayout* wrapLayout, RouteMenuButton* button);
			void addOptionButton(WrapLayout* wrapLayout, OptionMenuButton* button);
			void addWrapLayout(WrapLayout* wrapLayout);
			void addMFDInstrumentsModeButton(MFDModeMenuButton* button);

	};
}
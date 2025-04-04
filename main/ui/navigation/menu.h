#pragma once

#include "../../../components/yoba/src/main.h"
#include "../../../components/yoba/src/ui.h"
#include "../theme.h"

#include "menuItem.h"
#include "routes.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class MenuOverlayBackground : public FillColorElement {
		protected:
			void onEvent(Event* event) override;
	};

	class Menu : public Layout {
		public:
			explicit Menu();

			void updatePageItemsSelection();

		private:
			constexpr static const uint16_t _menuWidth = 140;

			MenuOverlayBackground overlayBackground;

			Layout _slideLayout;
			Rectangle _slideBackground;
			StackLayout _slideItemsLayout;

			TitleMenuItem
				_mainTitleItem = TitleMenuItem(L"Main"),
				_settingsTitleItem = TitleMenuItem(L"Settings"),
				_developerTitleItem = TitleMenuItem(L"Developer");

			PageMenuItem
				_mainMFDItem = PageMenuItem(L"MFD", &routes::MFD),
				_mainNDItem = PageMenuItem(L"NAV", &routes::ND),

				_settingsAxisItem = PageMenuItem(L"Axis calibration", &routes::settingsAxis),
				_settingsWiFiItem = PageMenuItem(L"Wi-Fi", &routes::settingsWiFi),
				_settingsUpdatesItem = PageMenuItem(L"Updates", &routes::settingsUpdates),

				_developerSpeakerItem = PageMenuItem(L"Speaker", &routes::developerSpeaker),
				_developerUITestItem = PageMenuItem(L"UI test", &routes::developerUITest);

			void addItem(MenuItem* item);

			void addTitleItem(TitleMenuItem* item);

			void addPageItem(PageMenuItem* item);
	};
}
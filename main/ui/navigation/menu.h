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
				_mainMFDItem = PageMenuItem(L"MFD", &Routes::MFD),

				_settingsAxisItem = PageMenuItem(L"Axis calibration", &Routes::settingsAxis),
				_settingsWiFiItem = PageMenuItem(L"Wi-Fi", &Routes::settingsWiFi),
				_settingsUpdatesItem = PageMenuItem(L"Updates", &Routes::settingsUpdates),

				_developerSpeakerItem = PageMenuItem(L"Speaker", &Routes::developerSpeaker),
				_developerUITestItem = PageMenuItem(L"UI test", &Routes::developerUITest);

			void addItem(MenuItem* item);

			void addTitleItem(TitleMenuItem* item);

			void addPageItem(PageMenuItem* item);
	};
}
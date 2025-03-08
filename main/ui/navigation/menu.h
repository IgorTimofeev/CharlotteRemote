#pragma once

#include "../../../components/yoba/src/ui.h"
#include "../theme.h"

#include "menuItem.h"
#include "routing.h"

#include "pfd/pfdPage.h"
#include "nd/ndPage.h"
#include "autopilot/autopilotPage.h"
#include "debug/debugPage.h"
#include "axis/axisPage.h"

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

			void updateItemsSelection();

		private:
			constexpr static const uint16_t _menuWidth = 140;

			MenuOverlayBackground overlayBackground;

			Layout _slideLayout;
			Rectangle _slideBackground;
			StackLayout _slideRows;
			StackLayout _slideItemsLayout;

			TitleMenuItem
				_mainTitleItem = TitleMenuItem(L"Main"),
				_settingsTitleItem = TitleMenuItem(L"Settings");

			PageMenuItem
				_PFDItem = PageMenuItem(L"PFD", &Routes::PFD),
				_NDItem = PageMenuItem(L"NAV", &Routes::ND),
				_APItem = PageMenuItem(L"Autopilot", &Routes::autopilot),
				_WiFiItem = PageMenuItem(L"Wi-Fi", &Routes::WiFi),
				_axisItem = PageMenuItem(L"Axis calibration", &Routes::axis),
				_debugItem = PageMenuItem(L"Developer", &Routes::debug);

			void addItem(MenuItem* item);

			void addTitleItem(TitleMenuItem* item);

			void addPageItem(PageMenuItem* item);
	};
}
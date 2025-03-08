#pragma once

#include "../../../components/yoba/src/ui.h"
#include "../theme.h"

#include "menuItem.h"
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

	class Menu : public Selector {
		public:
			explicit Menu();

		private:
			constexpr static const uint16_t _menuWidth = 100;

			MenuOverlayBackground overlayBackground;

			Layout _slideLayout;
			Rectangle _slideBackground;
			StackLayout _slideRows;
			Text _slideTitle;
			StackLayout _slideItemsLayout;

			TitleMenuItem _mainTitleItem = TitleMenuItem(L"Main");

			PageMenuItem _PFDItem = PageMenuItem(L"PFD", []() {
				return new PFDPage();
			});

			PageMenuItem _NDItem = PageMenuItem(L"N/D", []() {
				return new NDPage();
			});

			PageMenuItem _APItem = PageMenuItem(L"A/P", []() {
				return new AutopilotPage();
			});

			TitleMenuItem _settingsTitleItem = TitleMenuItem(L"Settings");

			PageMenuItem _axisItem = PageMenuItem(L"CTL", []() {
				return new AxisPage();
			});

			PageMenuItem _debugItem = PageMenuItem(L"DBG", []() {
				return new DebugPage();
			});
	};
}
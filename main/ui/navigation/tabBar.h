#pragma once

#include "../../../components/yoba/src/ui.h"
#include "../theme.h"

#include "tab.h"
#include "pfd/pfdPage.h"
#include "nd/ndPage.h"
#include "autopilot/autopilotPage.h"
#include "settings/settingsPage.h"

namespace pizdanc {
	class TabBar : public Selector {
		public:
			explicit TabBar();

		private:
			static const uint16_t _tabBarSize = 30;

			Layout _pageLayout;

			Layout _tabsLayoutHolder;
			Rectangle _tabsBackground = Rectangle(&Theme::bg2);
			EqualStackLayout _tabsLayout = EqualStackLayout(Orientation::horizontal);

			Tab _pfdTab = Tab(L"PFD", []() {
				return new PFDPage();
			});

			Tab _ndTab = Tab(L"N/D", []() {
				return new NDPage();
			});

			Tab _apTab = Tab(L"A/P", []() {
				return new AutopilotPage();
			});

			Tab _settingsTab = Tab(L"...", []() {
				return new SettingsPage();
			});
	};
}
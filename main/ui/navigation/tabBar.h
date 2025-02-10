#pragma once

#include "../../../components/yoba/src/ui.h"
#include "../theme.h"

#include "tab.h"
#include "pfd/pfdPage.h"
#include "nd/ndPage.h"
#include "autopilot/autopilotPage.h"
#include "debug/debugPage.h"
#include "controls/controlsPage.h"

namespace pizdanc {
	class TabBar : public Selector {
		public:
			explicit TabBar();

		private:
			static const uint16_t _tabBarSize = 26;

			Layout _pageLayout;

			Layout _tabsLayoutHolder;
			Rectangle _tabsBackground = Rectangle(&Theme::bg2);
			EqualStackLayout _tabsLayout = EqualStackLayout(Orientation::horizontal);

			Tab
				_pfdTab = Tab(L"PFD", []() {
					return new PFDPage();
				}),

				_ndTab = Tab(L"N/D", []() {
					return new NDPage();
				}),

				_apTab = Tab(L"A/P", []() {
					return new AutopilotPage();
				}),

				_controlsTab = Tab(L"CTL", []() {
					return new ControlsPage();
				}),

				_debugTab = Tab(L"DBG", []() {
					return new DebugPage();
				});
	};
}
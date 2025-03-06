#pragma once

#include "../../../components/yoba/src/ui.h"
#include "../theme.h"

#include "tab.h"
#include "pfd/pfdPage.h"
#include "nd/ndPage.h"
#include "autopilot/autopilotPage.h"
#include "debug/debugPage.h"
#include "axis/axisPage.h"

namespace pizda {
	class TabBar : public Selector {
		public:
			explicit TabBar();

		private:
			constexpr static const uint16_t _tabBarSize = 26;

			Layout _pageLayout;

			Layout _tabsLayoutHolder;
			Rectangle _tabsBackground = Rectangle(&Theme::bg2);
			EqualLayout _tabsLayout = EqualLayout(Orientation::horizontal);

			Tab
				_PFDTab = Tab(L"PFD", []() {
					return new PFDPage();
				}),

				_NDTab = Tab(L"N/D", []() {
					return new NDPage();
				}),

				_APTab = Tab(L"A/P", []() {
					return new AutopilotPage();
				}),

				_axisPage = Tab(L"CTL", []() {
					return new AxisPage();
				}),

				_debugTab = Tab(L"DBG", []() {
					return new DebugPage();
				});
	};
}
#pragma once

#include "../../../components/yoba/src/ui.h"
#include "../theme.h"

#include "tab.h"
#include "pfd/pfdPage.h"
#include "nd/ndPage.h"
#include "autopilot/autopilotPage.h"
#include "debug/debugPage.h"
#include "controls/controlsPage.h"

namespace pizda {
	class TabBar : public Selector {
		public:
			void setup();

		private:
			static const uint16_t _tabBarSize = 26;

			Layout _pageLayout;

			Layout _tabsLayoutHolder;
			Rectangle _tabsBackground = Rectangle(&Theme::bg2);
			EqualLayout _tabsLayout = EqualLayout(Orientation::horizontal);

			PFDPage _PFDPage;
			Tab _PFDTab = Tab(L"PFD", &_PFDPage);

			NDPage _NDPage;
			Tab _NDTab = Tab(L"N/D", &_NDPage);

			AutopilotPage _autopilotPage;
			Tab _autopilotTab = Tab(L"A/P", &_autopilotPage);

			ControlsPage _controlsPage;
			Tab _controlsTab = Tab(L"CTL", &_controlsPage);

			DebugPage _debugPage;
			Tab _debugTab = Tab(L"DBG", &_debugPage);
	};
}
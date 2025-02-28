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
			explicit TabBar();

			void setup() {
				// Page
				_pageLayout.setMargin(Margin(0, 0, 0, _tabBarSize));
				*this += &_pageLayout;

				// Tabs
				_tabsLayoutHolder.setHeight(_tabBarSize);
				_tabsLayoutHolder.setVerticalAlignment(Alignment::end);

				_tabsLayoutHolder += &_tabsBackground;

				_tabsLayout.setOrientation(Orientation::horizontal);
				_tabsLayoutHolder += &_tabsLayout;
				setItemsLayout(&_tabsLayout);

				*this += &_tabsLayoutHolder;

				// Callbacks
				selectionChanged += [this]() {
					auto selectedIndex = getSelectedIndex();

					if (selectedIndex < 0)
						return;

					auto tab = dynamic_cast<Tab*>(getItemAt(selectedIndex));

					// Removing old page
					if (_pageLayout.getChildrenCount() > 0) {
						auto oldPage = _pageLayout[0];
						_pageLayout.removeChildAt(0);
//						delete oldPage;
					}

					// Creating new page
					_pageLayout += tab->getPage();
				};

				// Initialization
				_PFDPage.setup();
				_NDPage.setup();
				_autopilotPage.setup();
				_controlsPage.setup();
				_debugPage.setup();

				addItem(&_PFDTab);
				addItem(&_NDTab);
				addItem(&_autopilotTab);
				addItem(&_controlsTab);
				addItem(&_debugTab);

				setSelectedIndex(1);
			}

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
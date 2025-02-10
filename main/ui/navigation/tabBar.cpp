#include "tabBar.h"
#include "settings/settingsPage.h"
#include "battery/batteryPage.h"
#include "pfd/pfdPage.h"
#include "nd/ndPage.h"
#include "autopilot/autopilotPage.h"
#include "radio/radioPage.h"
#include "controls/controlsPage.h"

namespace pizdanc {
	TabBar::TabBar() {
		// Page
		_pageLayout.setMargin(Margin(0, 0, 0, _tabsHeight));
		*this += &_pageLayout;

		// Tabs
		_tabsLayoutHolder.setHeight(_tabsHeight);
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

			auto tab = reinterpret_cast<Tab*>(getItemAt(selectedIndex));

			// Removing old page
			if (_pageLayout.getChildrenCount() > 0) {
				auto oldPage = dynamic_cast<Page*>(_pageLayout[0]);
				_pageLayout.removeChildAt(0);
				delete oldPage;
			}

			// Creating new page
			auto newPage = tab->getPageBuilder()();
			newPage->setup();
			_pageLayout += newPage;
		};

		// Initialization
		addItem(new Tab(L"PFD", []() {
			return new PFDPage();
		}));

		addItem(new Tab(L"N/D", []() {
			return new NDPage();
		}));

		addItem(new Tab(L"A/P", []() {
			return new AutopilotPage();
		}));

		addItem(new Tab(L"CTL", []() {
			return new ControlsPage();
		}));

		addItem(new Tab(L"COM", []() {
			return new RadioPage();
		}));

		addItem(new Tab(L"BAT", []() {
			return new BatteryPage();
		}));

		addItem(new Tab(L"SET", []() {
			return new SettingsPage();
		}));
	}
}
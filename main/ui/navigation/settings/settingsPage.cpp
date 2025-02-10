#include "settingsPage.h"
namespace pizdanc {
	uint8_t SettingsPage::_tabIndex = 0;

	SettingsPage::SettingsPage() {
		// Page
		_pageLayout.setMargin(Margin(0, tabBarSize, 0, 0));
		*this += &_pageLayout;

		// Tabs
		_tabsLayout.setOrientation(Orientation::horizontal);
		_tabBar.setItemsLayout(&_tabsLayout);

		_tabsLayoutHolder.setHeight(tabBarSize);
		_tabsLayoutHolder.setVerticalAlignment(Alignment::start);
		_tabsLayoutHolder += &_tabsBackground;
		_tabsLayoutHolder += &_tabsLayout;
		*this += &_tabsLayoutHolder;

		// Initialization
		_tabBar.selectionChanged += [this]() {
			auto selectedIndex = _tabBar.getSelectedIndex();

			if (selectedIndex < 0)
				return;

			_tabIndex = selectedIndex;

			auto tab = reinterpret_cast<SettingsPageTab*>(_tabBar.getItemAt(selectedIndex));

			// Removing old page
			if (_pageLayout.getChildrenCount() > 0) {
				auto oldPage = _pageLayout[0];
				_pageLayout.removeChildAt(0);
				delete oldPage;
			}

			// Creating new page
			auto newPage = tab->getPageBuilder()();
			newPage->setup();
			_pageLayout += newPage;
		};

		_tabBar.addItem(&_debugTab);
		_tabBar.addItem(&_radioTab);
		_tabBar.addItem(&_controlsTab);
		_tabBar.addItem(&_batteryTab);
		_tabBar.setSelectedIndex(_tabIndex);
	}

	SettingsPage::~SettingsPage() {
		auto selectedIndex = _tabBar.getSelectedIndex();

		if (selectedIndex >= 0) {
			delete _pageLayout[0];
		}
	}
}
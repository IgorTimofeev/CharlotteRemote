#include "tabBar.h"

namespace pizdanc {
	TabBar::TabBar() {
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

			auto tab = reinterpret_cast<Tab*>(getItemAt(selectedIndex));

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

		// Initialization
		addItem(&_pfdTab);
		addItem(&_ndTab);
		addItem(&_apTab);
		addItem(&_controlsTab);
		addItem(&_debugTab);
	}
}
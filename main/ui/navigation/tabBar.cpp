#include "tabBar.h"

namespace pizda {
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

			auto tab = dynamic_cast<Tab*>(getItemAt(selectedIndex));

			// Removing old page
			if (_pageLayout.getChildrenCount() > 0) {
				auto oldPage = _pageLayout[0];
				_pageLayout.removeChildAt(0);

				delete oldPage;
			}

			// Creating new page
			_pageLayout += tab->newPage();
		};

		// Initialization
		addItem(&_PFDTab);
		addItem(&_NDTab);
		addItem(&_APTab);
		addItem(&_axisPage);
		addItem(&_debugTab);
	}
}
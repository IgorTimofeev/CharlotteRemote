#include "tabBar.h"

namespace pizda {
	void TabBar::setup() {
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
			if (_pageLayout.getChildrenCount() > 0)
				_pageLayout.removeChildAt(0);

			// Creating new page
			_pageLayout += tab->getPage();
		};

		// Initialization
		_autopilotPage.setup();
		_debugPage.setup();

		addItem(&_PFDTab);
		addItem(&_NDTab);
		addItem(&_autopilotTab);
		addItem(&_controlsTab);
		addItem(&_debugTab);

		setSelectedIndex(0);
	}
}
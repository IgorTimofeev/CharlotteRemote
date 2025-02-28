#include <esp_log.h>
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
			auto newPage = tab->buildPage();
			newPage->setup();
			_pageLayout += newPage;

			if (heap_caps_check_integrity_all(true)) {
				ESP_LOGI("HEAP", "heap_caps_check_integrity_all passed");
			}
			else {
				ESP_LOGE("HEAP", "heap_caps_check_integrity_all failed");
			}
		};

		// Initialization
		addItem(&_pfdTab);
		addItem(&_ndTab);
		addItem(&_apTab);
		addItem(&_controlsTab);
		addItem(&_debugTab);
	}
}
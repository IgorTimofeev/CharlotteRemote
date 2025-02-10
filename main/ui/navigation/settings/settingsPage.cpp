#include "settingsPage.h"
#include "settingsPageTab.h"
#include "debug/debugPage.h"

namespace pizdanc {
	SettingsPage::SettingsPage() {
		// Page
		_pageLayout.setMargin(Margin(tabBarWidth, 0, 0, 0));
		*this += &_pageLayout;

		// Tabs
		_tabsLayout.setOrientation(Orientation::horizontal);
		_tabBar.setItemsLayout(&_tabsLayout);

		_tabsLayoutHolder.setWidth(tabBarWidth);
		_tabsLayoutHolder.setHorizontalAlignment(Alignment::start);
		_tabsLayoutHolder += &_tabsBackground;
		_tabsLayoutHolder += &_tabsLayout;
		*this += &_tabsLayoutHolder;

		// Initialization
		_tabBar.selectionChanged += [this]() {
			auto selectedIndex = _tabBar.getSelectedIndex();

			if (selectedIndex < 0)
				return;

			auto selectedItem = reinterpret_cast<SettingsPageTab*>(_tabBar.getItemAt(selectedIndex));

			// Removing old page
			if (_pageLayout.getChildrenCount() > 0) {
				auto oldPage = dynamic_cast<Page*>(_pageLayout[0]);
				_pageLayout.removeChildAt(0);
				delete oldPage;
			}

			// Creating new page
			auto newPage = selectedItem->getPageBuilder()();
			newPage->setup();
			_pageLayout += newPage;
		};

		_tabBar.addItem(new SettingsPageTab(L"DBG", []() {
			return new DebugPage();
		}));
	}

	SettingsPage::~SettingsPage() {
		
	}
}
#include "menu.h"

namespace pizdanc {
	void Menu::setup() {
		*this += &_background;

		// Menu
		const uint16_t menuHeight = 20;
		_menu.setSize(Size(Size::computed, menuHeight));
		_menu.setVerticalAlignment(Alignment::end);
		_menu += &_menuBackground;

		_menuItemsLayout.setOrientation(Orientation::horizontal);
		_menu += &_menuItemsLayout;
		setItemsLayout(&_menuItemsLayout);

		*this += &_menu;

		// Page
		_pageLayout.setMargin(Margin(0, 0, 0, menuHeight));
		*this += &_pageLayout;

		// Initialization
		addPage(L"DBG", &_debugPage);
		addPage(L"PFD", &_pfdPage);
		addPage(L"A/P", &_autopilotPage);
		addPage(L"CTL", &_controlsPage);
		addPage(L"ENG", &_enginePage);
		addPage(L"COM", &_radioPage);
		addPage(L"BAT", &_batteryPage);

		setSelectedIndex(0);
	}

	void Menu::onSelectionChanged() {
		_pageLayout.removeChildren();

		if (getSelectedIndex() < 0)
			return;

		_pageLayout += _pages[getSelectedIndex()];
	}

	void Menu::addPage(const std::wstring_view& name, Page *page) {
		page->setup();
		_pages.push_back(page);
		addItem(new MenuItem(name));
	}
}
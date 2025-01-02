#include "menu.h"
#include "../rc_application.h"

namespace pizdanc {
	void Menu::setup() {
		*this += &_background;

		// Menu
		const uint16_t menuSize = 20;
		_menu.setSize(Size(Size::Auto, menuSize));
		_menu.setVerticalAlignment(Alignment::End);
		_menu += &_menuBackground;

		_menuItemsLayout.setOrientation(Orientation::Horizontal);
		_menu += &_menuItemsLayout;
		setItemsLayout(&_menuItemsLayout);

		*this += &_menu;

		// Page
		_pageLayout.setMargin(Margin(0, 0, 0, menuSize));
		*this += &_pageLayout;

		// Initialization
		addPage(U"DBG", &_debugPage);
		addPage(U"PFD", &_pfdPage);
		addPage(U"A/P", &_autopilotPage);
		addPage(U"CTU", &_controlsPage);
		addPage(U"ENG", &_enginePage);
		addPage(U"COM", &_radioPage);
		addPage(U"BAT", &_batteryPage);

		setSelectedIndex(0);
	}

	void Menu::onSelectionChanged() {
		_pageLayout.removeChildren();

		if (getSelectedIndex() < 0)
			return;

		_pageLayout += _pages[getSelectedIndex()];
	}

	void Menu::addPage(const std::u32string_view& name, Page *page) {
		page->setup();
		_pages.push_back(page);
		addItem(new MenuItem(name));
	}
}
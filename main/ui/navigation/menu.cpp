#include "menu.h"
#include "debug/debugPage.h"
#include "battery/batteryPage.h"
#include "pfd/pfdPage.h"
#include "autopilot/autopilotPage.h"
#include "radio/radioPage.h"
#include "controls/controlsPage.h"
#include "../../rc.h"

namespace pizdanc {
	Menu::Menu() {
		// Page
		_pageLayout.setMargin(Margin(0, 0, 0, selectorHeight));
		*this += &_pageLayout;

		// Items
		_itemsHolder.setHeight(selectorHeight);
		_itemsHolder.setVerticalAlignment(Alignment::end);

		_itemsHolder += &_itemsBackground;

		_itemsLayout.setOrientation(Orientation::horizontal);
		_itemsHolder += &_itemsLayout;
		setItemsLayout(&_itemsLayout);

		*this += &_itemsHolder;

		// Initialization
//		addPage(L"PFD", &_PFDPage);
//		addPage(L"A/P", &_autopilotPage);
//		addPage(L"CTL", &_controlsPage);
//		addPage(L"COM", &_radioPage);
//		addPage(L"BAT", &_batteryPage);
//		addPage(L"DBG", &_debugPage);

		addItem(new MenuItem(L"PFD", []() {
			return new PFDPage();
		}));

		addItem(new MenuItem(L"A/P", []() {
			return new AutopilotPage();
		}));

		addItem(new MenuItem(L"CTL", []() {
			return new ControlsPage();
		}));

		addItem(new MenuItem(L"COM", []() {
			return new RadioPage();
		}));

		addItem(new MenuItem(L"BAT", []() {
			return new BatteryPage();
		}));

		addItem(new MenuItem(L"DBG", []() {
			return new DebugPage();
		}));
	}

	void Menu::onSelectionChanged() {
		auto selectedIndex = getSelectedIndex();

		if (selectedIndex < 0)
			return;

		auto selectedItem = reinterpret_cast<MenuItem*>(getItemAt(selectedIndex));

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

		// Settings
		auto& settings = RC::getInstance().getSettings();

		if (selectedIndex == settings.menuPageIndex)
			return;

		settings.menuPageIndex = selectedIndex;
		settings.enqueueWrite();
	}
}
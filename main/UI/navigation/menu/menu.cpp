#include "menu.h"
#include "rc.h"

namespace pizda {
	Menu::Menu() {
		// Tabs
		_tabsRow.setOrientation(Orientation::horizontal);

		for (auto& tab : tabs)
			_tabsRow += &tab;

		// Tabs & content
		_tabsAndContentRows += &_tabsRow;
		slideLayout += &_tabsAndContentRows;

		setViewRoute(_viewRoute);
	}

	Menu::~Menu() {
		delete _view;
	}

	const Route* Menu::_viewRoute = &MenuRoutes::MFD;

	void Menu::setViewRoute(const Route* route) {
		_viewRoute = route;

		for (auto& tab : tabs) {
			tab.setActive(tab.getRoute() == route);
		}

		if (_view) {
			_tabsAndContentRows -= _view;
			delete _view;
		}

		_view = dynamic_cast<MenuView*>(_viewRoute->buildElement());
		_view->setup();
		_tabsAndContentRows.insertChild(0, _view);

		auto& rc = RC::getInstance();

		if (_view->getRoute() != rc.getRoute())
			rc.setRoute(_view->getRoute());
	}
}
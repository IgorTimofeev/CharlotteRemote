#include "menu.h"
#include "../../../rc.h"
#include "../MFD/MFDPage.h"

namespace pizda {
	MenuOverlayBackground::MenuOverlayBackground() {
		setFillColor(&Theme::bg1);
	}

	void MenuOverlayBackground::onEvent(Event* event) {
		Element::onEvent(event);

		if (!ScreenEvent::isScreen(event))
			return;

		event->setHandled(true);

		RC::getInstance().hideMenu();
	}

	Menu::Menu() {
		// Background
		*this += &_overlayBackground;

		// Holder

		// Items background
		_slideBackground.setFillColor(&Theme::bg2);
		_slideLayout += &_slideBackground;

		// Tabs
		_tabsRow.setOrientation(Orientation::horizontal);

		for (auto& tab : tabs)
			_tabsRow += &tab;

		// Tabs & content
		_tabsAndContentRows += &_tabsRow;
		_slideLayout += &_tabsAndContentRows;

		_slideLayout.setVerticalAlignment(Alignment::end);
		setAutoSize(&_slideLayout);
		*this += &_slideLayout;

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
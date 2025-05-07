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
		auto& rc = RC::getInstance();

		// Background
		*this += &_overlayBackground;

		// Holder
		_slideLayout.setVerticalAlignment(Alignment::end);

		// Items background
		_slideBackground.setFillColor(&Theme::bg2);
		_slideLayout += &_slideBackground;

		setAutoSize(&_slideLayout);
		*this += &_slideLayout;

		// Rows
		_tabsRow.setOrientation(Orientation::horizontal);

		_tabsAndContentRows += &_tabsRow;
		_slideLayout += &_tabsAndContentRows;

		for (auto tab : tabs)
			_tabsRow += tab;

		setTabRoute(_tabRoute);
	}

	Menu::~Menu() {
		if (_tabView) {
			delete _tabView;
		}
	}

	const Route* Menu::_tabRoute = &MenuRoutes::MFD;

	void Menu::setTabRoute(const Route* route) {
		_tabRoute = route;

		for (auto tab : tabs) {
			tab->setChecked(tab->getRoute() == route);
		}

		if (_tabView) {
			_tabsAndContentRows -= _tabView;
			delete _tabView;
		}

		if (_tabRoute) {
			_tabView = _tabRoute->buildElement();
			_tabsAndContentRows += _tabView;
		}
		else {
			_tabView = nullptr;
		}
	}
}
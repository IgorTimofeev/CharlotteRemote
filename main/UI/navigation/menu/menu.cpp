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

	Menu::Menu(const Route* route) {
		// Background
		*this += &_overlayBackground;

		// Holder
		_slideLayout.setVerticalAlignment(Alignment::end);

		// Items background
		_slideBackground.setFillColor(&Theme::bg2);
		_slideLayout += &_slideBackground;

		setAutoSize(&_slideLayout);
		*this += &_slideLayout;

		setView(route);
	}

	Menu::~Menu() {
		if (_view) {
			delete _view;
		}
	}

	void Menu::setView(const Route* value) {
		_route = value;

		if (_view) {
			_slideLayout -= _view;
			delete _view;
		}

		_view = _route->buildElement();
		_slideLayout += _view;
	}
}
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
		_rows.setSpacing(10);
		_rows.setMargin(Margin(15, 10, 15, 10));
		_slideLayout += &_rows;

		// Lower section
		_lowerSection.title.setText(L"Menu");

		for (auto routeButton : _routeButtons) {
			if (rc.getRoute() == routeButton->getMainRoute()) {
				routeButton->setChecked(true);

				if (routeButton->getUpperSectionRoute() != nullptr)
					addUpperSectionRoute(routeButton->getUpperSectionRoute());
			}

			_lowerSection.wrapLayout += routeButton;
		}

		_rows += &_lowerSection;
	}

	Menu::~Menu() {
		if (_upperSectionElement) {
			delete _upperSectionElement;
		}
	}

	void Menu::removeUpperSectionRoute() {
		_upperSectionRoute = nullptr;

		if (_upperSectionElement) {
			_rows -= _upperSectionElement;
			delete _upperSectionElement;
			_upperSectionElement = nullptr;
		}
	}

	void Menu::addUpperSectionRoute(const Route* route) {
		_upperSectionRoute = route;

		_upperSectionElement = _upperSectionRoute->buildElement();
		_rows.insertChild(0, _upperSectionElement);
	}

	void Menu::setRoute(const Route* route) {
		auto& rc = RC::getInstance();

		rc.setRoute(route);
		removeUpperSectionRoute();

		for (auto routeButton : _routeButtons) {
			if (routeButton->getMainRoute() == route) {
				routeButton->setChecked(true);

				if (routeButton->getUpperSectionRoute() != nullptr)
					addUpperSectionRoute(routeButton->getUpperSectionRoute());
			}
			else {
				routeButton->setChecked(false);
			}
		}
	}
}
#include "pagesMenuView.h"

#include "../../theme.h"
#include "../../../rc.h"

namespace pizda {
	PagesMenuView::PagesMenuView(PageMenuViewButton* buttons, uint8_t buttonCount, const Route** lastRoute) :
		_lastRoute(lastRoute),
		_buttons(buttons),
		_buttonCount(buttonCount)
	{

	}

	void PagesMenuView::setup() {
		MenuView::setup();

		if (*_lastRoute == nullptr)
			*_lastRoute = _buttons[0].getRoute();

		for (uint8_t i = 0; i < _buttonCount; i++) {
			const auto button = _buttons + i;

			if (button->getRoute() == *_lastRoute) {
				button->setActive(true);
			}

			*this += button;
		}
	}

	const Route* PagesMenuView::getRoute() {
		return *_lastRoute;
	}

	void PagesMenuView::setRoute(const Route* route) const {
		*_lastRoute = route;

		for (uint8_t i = 0; i < _buttonCount; i++) {
			const auto button = _buttons + i;

			button->setActive(button->getRoute() == *_lastRoute);
		}

		RC::getInstance().setRoute(*_lastRoute);
	}
}

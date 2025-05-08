#include "pagesMenuView.h"

#include "../../theme.h"
#include "../../../rc.h"

namespace pizda {
	PagesMenuView::PagesMenuView() {
		if (_route == nullptr)
			_route = _buttons[0]->getRoute();

		for (const auto button : _buttons) {
			if (button->getRoute() == _route) {
				button->setChecked(true);
			}

			*this += button;
		}

		// Power
		_powerButton.click += [] {
			esp_restart();
		};

		*this += &_powerButton;
	}

	const Route* PagesMenuView::_route = nullptr;

	const Route* PagesMenuView::getRoute() {
		return _route;
	}

	void PagesMenuView::setRoute(const Route* route) {
		_route = route;

		for (const auto button : _buttons) {
			button->setChecked(button->getRoute() == _route);
		}

		RC::getInstance().setRoute(_route);
	}
}
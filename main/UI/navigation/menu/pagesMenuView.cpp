#include "MFDMenuView.h"

#include "../../theme.h"
#include "../../../rc.h"

namespace pizda {
	PagesMenuView::PagesMenuView() {
		auto& rc = RC::getInstance();

		for (auto button : _buttons) {
			if (button->getRoute() == rc.getRoute()) {
				button->setChecked(true);
			}

			*this += button;
		}
	}

	void PagesMenuView::setRoute(const Route* route) {
		auto& rc = RC::getInstance();
		rc.setRoute(route);

		for (auto button : _buttons) {
			button->setChecked(button->getRoute() == route);
		}
	}
}
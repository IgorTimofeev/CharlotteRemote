#include "settingsMenuView.h"

#include "../../theme.h"
#include "../../../rc.h"

namespace pizda {
	SettingsMenuView::SettingsMenuView() : PagesMenuView(_pageButtons, 5, &_lastRoute) {

	}

	const Route* SettingsMenuView::_lastRoute = nullptr;

	void SettingsMenuView::setup() {
		PagesMenuView::setup();

		// Power
		_powerButton.click += [] {
			esp_restart();
		};

		*this += &_powerButton;
	}
}
#include "settingsMenuView.h"

#include <UI/theme.h>
#include "rc.h"

namespace pizda {
	SettingsMenuView::SettingsMenuView() : PagesMenuView(_pageButtons.data(), _pageButtons.size(), &_lastRoute) {

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
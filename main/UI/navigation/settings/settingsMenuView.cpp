#include "settingsMenuView.h"

#include "UI/theme.h"
#include "rc.h"

namespace pizda {
	const Route* SettingsMenuView::_lastRoute = nullptr;
	
	void SettingsMenuView::setup() {
		MenuView::setup();
		
		setupButtons(_buttons.data(), _buttons.size(), &_lastRoute);
	}
}
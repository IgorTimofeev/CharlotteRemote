#include "settingsMenuView.h"

#include "UI/theme.h"
#include "rc.h"

namespace pizda {
	SettingsMenuView::SettingsMenuView() : PagesMenuView(_pageButtons.data(), _pageButtons.size(), &_lastRoute) {

	}

	const Route* SettingsMenuView::_lastRoute = nullptr;
}
#include "NavMenuView.h"

#include "UI/theme.h"
#include "rc.h"

namespace pizda {
	const Route* NavMenuView::_lastRoute = nullptr;
	
	void NavMenuView::setup() {
		MenuView::setup();
		
		setupButtons(_buttons.data(), _buttons.size(), &_lastRoute);
	}
}
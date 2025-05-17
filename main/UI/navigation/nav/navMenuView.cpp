#include "NavMenuView.h"

#include <UI/theme.h>
#include <rc.h>

namespace pizda {
	NavMenuView::NavMenuView() : PagesMenuView(_buttons.data(), _buttons.size(), &_lastRoute) {

	}

	const Route* NavMenuView::_lastRoute = nullptr;
}
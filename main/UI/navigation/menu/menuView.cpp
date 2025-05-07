#include "menuView.h"

#include "menu.h"
#include "../../theme.h"

namespace pizda {
	MenuView::MenuView() {
		setSpacing(10);
		setMargin(Margin(15, 15, 15, 10));
	}

	Menu* MenuView::getMenu() {
		return reinterpret_cast<Menu*>(getParent()->getParent()->getParent());
	}
}
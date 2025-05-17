#include "menuView.h"

#include "menu.h"
#include <UI/theme.h>

namespace pizda {
	MenuView::MenuView() {
		setSpacing(10);
		setMargin(Margin(15, 15, 15, 10));
	}

	void MenuView::setup() {

	}

	Menu* MenuView::getMenu() const {
		return reinterpret_cast<Menu*>(getParent()->getParent()->getParent());
	}
}
#include "menuView.h"

#include "../../theme.h"
#include "../../../settings.h"
#include "menu.h"

namespace pizda {
	MenuView::MenuView() {
		setSpacing(10);
		setMargin(Margin(15, 10, 15, 15));

		Theme::apply(&title);
		*this += &title;

		wrapLayout.setSpacing(10);
		*this += &wrapLayout;
	}

	Menu* MenuView::getMenu() {
		return reinterpret_cast<Menu*>(getParent()->getParent());
	}
}
#include "menuSection.h"

#include "../../theme.h"
#include "../../../settings.h"
#include "menu.h"

namespace pizda {
	MenuSection::MenuSection() {
		setSpacing(10);

		Theme::apply(&title);
		*this += &title;

		wrapLayout.setSpacing(10);
		*this += &wrapLayout;
	}

	Menu* MenuSection::getMenu() {
		return reinterpret_cast<Menu*>(getParent()->getParent()->getParent());
	}
}
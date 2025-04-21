#include "menuView.h"

#include "../../theme.h"
#include "../../../settings.h"

namespace pizda {
	MenuView::MenuView() {
		setSpacing(10);
		setMargin(Margin(15, 10, 15, 15));

		Theme::applyTitle(&title);
		*this += &title;

		wrapLayout.setSpacing(10);
		*this += &wrapLayout;
	}
}
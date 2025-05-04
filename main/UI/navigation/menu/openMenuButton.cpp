#include "openMenuButton.h"

#include "../../theme.h"
#include "../../../rc.h"
#include "menuRoutes.h"

namespace pizda {
	OpenMenuButton::OpenMenuButton(const Route* route) : _route(route) {
		Theme::apply(this);

		setSize(Size(24));
		setCornerRadius(getSize().getHeight() / 2);

		setDefaultBackgroundColor(&Theme::bg3);
		setDefaultTextColor(&Theme::fg2);

		setPressedBackgroundColor(&Theme::bg1);
		setPressedTextColor(&Theme::fg1);

		setText(L"...");
	}

	OpenMenuButton::OpenMenuButton() : OpenMenuButton(&MenuRoutes::main) {

	}

	void OpenMenuButton::onClick() {
		Button::onClick();

		RC::getInstance().showMenu(_route);
	}
}
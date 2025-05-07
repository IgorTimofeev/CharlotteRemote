#include "openMenuButton.h"

#include "../../theme.h"
#include "../../../rc.h"

namespace pizda {
	OpenMenuButton::OpenMenuButton() {
		Theme::apply(this);

		setSize(Size(24));
		setCornerRadius(getSize().getHeight() / 2);

		setDefaultBackgroundColor(&Theme::bg3);
		setDefaultTextColor(&Theme::fg2);

		setPressedBackgroundColor(&Theme::bg1);
		setPressedTextColor(&Theme::fg1);

		setText(L"...");
	}

	void OpenMenuButton::onClick() {
		Button::onClick();

		RC::getInstance().showMenu();
	}
}
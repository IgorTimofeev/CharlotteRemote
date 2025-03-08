#include "menuButton.h"

#include "../theme.h"
#include "../../rc.h"

namespace pizda {
	MenuButton::MenuButton() {
		Theme::apply(this);

		setSize(Size(20));
		setCornerRadius(getSize().getHeight() / 2);

		setDefaultBackgroundColor(&Theme::bg3);
		setDefaultTextColor(&Theme::fg2);

		setPressedBackgroundColor(&Theme::bg1);
		setPressedTextColor(&Theme::fg1);

		setText(L"<");
	}

	void MenuButton::onPressedChanged() {
		Button::onPressedChanged();

		if (!isPressed())
			return;

		RC::getInstance().setMenuVisibility(true);
	}
}
#include "openMenuButton.h"

#include "../theme.h"
#include "../../rc.h"

namespace pizda {
	OpenMenuButton::OpenMenuButton() {
		Theme::apply(this);

		setSize(Size(22));
		setCornerRadius(getSize().getHeight() / 2);

		setDefaultBackgroundColor(&Theme::bg3);
		setDefaultTextColor(&Theme::fg2);

		setPressedBackgroundColor(&Theme::bg1);
		setPressedTextColor(&Theme::fg1);

//		setFontScale(2);
		setText(L"<");
	}

	void OpenMenuButton::onIsCheckedChanged() {
		Button::onIsCheckedChanged();

		if (!isChecked())
			return;

		RC::getInstance().setMenuVisibility(true);
	}
}
#include "openMenuButton.h"

#include "../../theme.h"
#include "../../../rc.h"

namespace pizda {
	void OpenMenuButton::onClick() {
		Button::onClick();

		RC::getInstance().showMenu();
	}

	void OpenMenuButton::applyRoundStyle() {
		setSize(Size(_roundStyleSize));
		setCornerRadius(getSize().getWidth() / 2 - 1);

		setDefaultBackgroundColor(&Theme::bg3);
		setDefaultTextColor(&Theme::fg3);

		setPressedBackgroundColor(&Theme::fg1);
		setPressedTextColor(&Theme::bg1);

		setFont(&Theme::fontSmall);

		setText(L"^");
	}

	void OpenMenuButton::applyBottomStyle() {
		applyRoundStyle();

		setAlignment(Alignment::center, Alignment::end);
		setMargin(Margin(0, 0, 0, -(_roundStyleSize / 2 + 3)));
		setContentMargin(Margin(0, 0, 0, 7));
	}

	void OpenMenuButton::applyBottomRightStyle() {
		applyRoundStyle();

		setAlignment(Alignment::end, Alignment::end);
		setMargin(Margin(0, 0, -(_roundStyleSize / 2 + 3), 50 / 2 - _roundStyleSize / 2));
		setContentMargin(Margin(0, 0, 7, 0));
	}
}

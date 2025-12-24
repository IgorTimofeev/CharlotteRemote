#include "openMenuButton.h"

#include <UI/theme.h>
#include "rc.h"

#include <UI/navigation/menu/menu.h>

namespace pizda {
	void OpenMenuButton::onClick() {
		Button::onClick();

		(new Menu())->show();
	}

	void OpenMenuButton::applyRoundStyle() {
		setSize(Size(_roundStyleSize));
		setCornerRadius(getSize().getWidth() / 2 - 1);

		setDefaultBackgroundColor(&Theme::bg3);
		setDefaultTextColor(&Theme::fg3);

		setActiveBackgroundColor(&Theme::fg1);
		setActiveTextColor(&Theme::bg1);

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
		setMargin(Margin(0, 0, -(_roundStyleSize / 2 + 3), 5));
		setContentMargin(Margin(0, 0, 7, 0));
	}
}

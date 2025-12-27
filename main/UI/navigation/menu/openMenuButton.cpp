#include "openMenuButton.h"

#include "UI/theme.h"
#include "rc.h"

#include <UI/navigation/menu/menu.h>

namespace pizda {
	OpenMenuButton::OpenMenuButton() {
		constexpr static uint8_t size = 34;
		
		setSize(Size(size));
		setCornerRadius(getSize().getWidth() / 2 - 1);
		
		setDefaultBackgroundColor(&Theme::bg3);
		setDefaultTextColor(&Theme::fg4);
		
		setActiveBackgroundColor(&Theme::fg1);
		setActiveTextColor(&Theme::bg1);
		
		setFont(&Theme::fontSmall);
		
		setText(L"^");
		
		setAlignment(Alignment::center, Alignment::end);
		setMargin(Margin(0, 0, 0, -(size / 2 + 3)));
		setContentMargin(Margin(0, 0, 0, 7));
	}
	
	void OpenMenuButton::onClick() {
		Button::onClick();

		(new Menu())->show();
	}
}

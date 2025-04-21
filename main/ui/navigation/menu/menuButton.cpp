#include "menuButton.h"

#include "menuView.h"
#include "menu.h"

#include "../../../rc.h"

namespace pizda {
	// -------------------------------- Default --------------------------------

	MenuButton::MenuButton(const std::wstring_view& text) {
		setSize(Size(45, 45));
		setText(text);

		setDefaultBorderColor(&Theme::bg4);
		setPressedBorderColor(&Theme::accent1);

		setDefaultTextColor(&Theme::fg3);
		setPressedTextColor(&Theme::fg1);
	}

	void MenuButton::onRender(Renderer* renderer, const Bounds& bounds) {
		const Color* color;

		if (isChecked()) {
			color = getPressedBorderColor();
		}
		else if (isFocused()) {
			color = &Theme::fg5;
		}
		else {
			color = getDefaultBorderColor();
		}

		// Border
		renderer->renderRectangle(bounds, 3, color);

		// Text
		renderer->renderString(
			Point(
				bounds.getXCenter() - Theme::fontSmall.getWidth(getText()) / 2,
				bounds.getY2() - Theme::fontSmall.getHeight() - 2
			),
			&Theme::fontSmall,
			isChecked() ? getPressedTextColor() : getDefaultTextColor(),
			getText()
		);
	}

	MenuView* MenuButton::getMenuView() {
		return reinterpret_cast<MenuView*>(getParent()->getParent());
	}

	// -------------------------------- View --------------------------------

	ViewMenuButton::ViewMenuButton(const std::wstring_view& text, const Route* route) : MenuButton(text), _route(route) {

	}

	void ViewMenuButton::onClick() {
		Button::onClick();

		RC::getInstance().getApplication().enqueueOnTick([this]() {
			getMenuView()->getMenu()->setView(_route);
		});
	}
}
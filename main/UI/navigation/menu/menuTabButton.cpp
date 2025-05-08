#include "menuTabButton.h"

#include "menuView.h"
#include "menu.h"

namespace pizda {
	MenuTabButton::MenuTabButton(std::wstring_view text, const Route* route) : _route(route) {
		setHeight(16);

		setDefaultBackgroundColor(&Theme::bg1);
		setDefaultTextColor(&Theme::fg5);

		setPressedBackgroundColor(&Theme::bg2);
		setPressedTextColor(&Theme::fg1);

		setFont(&Theme::fontSmall);
		setText(text);
	}

	void MenuTabButton::onRender(Renderer* renderer) {
		Button::onRender(renderer);

		// if (isChecked())
		// 	renderer->renderHorizontalLine(bounds.getBottomLeft(), bounds.getWidth(), &Theme::fg1);
	}

	void MenuTabButton::onClick() {
		Button::onClick();

		reinterpret_cast<Menu*>(getParent()->getParent()->getParent()->getParent())->setViewRoute(_route);
	}

	const Route* MenuTabButton::getRoute() const {
		return _route;
	}
}
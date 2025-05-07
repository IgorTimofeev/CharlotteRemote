#include "menuTabButton.h"

#include "menuView.h"
#include "menu.h"

namespace pizda {
	MenuTabButton::MenuTabButton(const std::wstring_view& text, const Route* route) : _route(route) {
		setHeight(18);

		setDefaultBackgroundColor(&Theme::bg3);
		setDefaultTextColor(&Theme::fg4);

		setPressedBackgroundColor(&Theme::bg4);
		setPressedTextColor(&Theme::fg1);

		setFont(&Theme::fontSmall);
		setText(text);
	}

	const Route* MenuTabButton::getRoute() const {
		return _route;
	}

	void MenuTabButton::onClick() {
		Button::onClick();

		reinterpret_cast<Menu*>(getParent()->getParent()->getParent()->getParent())->setTabRoute(_route);
	}

	void MenuTabButton::onRender(Renderer* renderer, const Bounds& bounds) {
		Button::onRender(renderer, bounds);

		if (isChecked())
			renderer->renderHorizontalLine(bounds.getBottomLeft(), bounds.getWidth(), &Theme::fg1);
	}
}
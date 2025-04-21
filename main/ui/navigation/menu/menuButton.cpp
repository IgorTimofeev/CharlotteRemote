#include "menuButton.h"

#include "menuView.h"
#include "menu.h"

#include "../../../rc.h"

namespace pizda {
	// -------------------------------- Default --------------------------------

	MenuButton::MenuButton(const Image* image, const std::wstring_view& text) : _image(image) {
		setSize(Size(45, image->getSize().getHeight() + _textOffset + Theme::fontSmall.getHeight()));
		setText(text);

		setDefaultBackgroundColor(&Theme::fg4);
		setPressedBackgroundColor(&Theme::fg1);
	}

	void MenuButton::onRender(Renderer* renderer, const Bounds& bounds) {
		const auto color = isChecked() ? getPressedBackgroundColor() : getDefaultBackgroundColor();

		// Image
		renderer->renderImage(bounds.getTopLeft(), _image);

		// Border
		if (isChecked()) {
			renderer->renderRectangle(
				Bounds(bounds.getX() - 1, bounds.getY() - 1, bounds.getWidth() + 2, _image->getSize().getHeight() + 2),
				4,
				color
			);
		}

		// Text
		renderer->renderString(
			Point(
				bounds.getXCenter() - Theme::fontSmall.getWidth(getText()) / 2,
				bounds.getY() + _image->getSize().getHeight() + _textOffset
			),
			&Theme::fontSmall,
			color,
			getText()
		);
	}

	MenuView* MenuButton::getMenuView() {
		return reinterpret_cast<MenuView*>(getParent()->getParent());
	}

	// -------------------------------- View --------------------------------

	ViewMenuButton::ViewMenuButton(const Image* image, const std::wstring_view& text, const Route* route) : MenuButton(image, text), _route(route) {

	}

	void ViewMenuButton::onClick() {
		Button::onClick();

		RC::getInstance().getApplication().enqueueOnTick([this]() {
			getMenuView()->getMenu()->setView(_route);
		});
	}
}
#include "menuViewButton.h"

#include "menuView.h"
#include "menu.h"

#include "rc.h"

namespace pizda {
	// -------------------------------- Default --------------------------------

	MenuViewButton::MenuViewButton(const Image* image, std::wstring_view text) : _image(image) {
		setSize(Size(45, image->getSize().getHeight() + _textOffset + Theme::fontSmall.getHeight()));
		setText(text);

		setDefaultBackgroundColor(Theme::bg3);
		setDefaultTextColor(Theme::fg4);

		setActiveBackgroundColor(Theme::bg3);
		setActiveBorderColor(Theme::fg1);
		setActiveTextColor(Theme::fg1);
	}

	void MenuViewButton::onRender(Renderer& renderer, const Bounds& bounds) {
		constexpr static uint8_t cornerRadius = 3;

		// Background
		renderer.renderFilledRectangle(
			Bounds(bounds.getX() - 1, bounds.getY() - 1, bounds.getWidth() + 2, _image->getSize().getHeight() + 2),
			cornerRadius,
			isActive() ? getActiveBackgroundColor() : getDefaultBackgroundColor()
		);

		// Image
		renderer.renderImage(bounds.getTopLeft(), _image);

		// Border
		if (isActive()) {
			renderer.renderRectangle(
				Bounds(bounds.getX() - 1, bounds.getY() - 1, bounds.getWidth() + 2, _image->getSize().getHeight() + 2),
				cornerRadius,
				getActiveBorderColor()
			);
		}

		// Text
		renderer.renderText(
			Point(
				bounds.getXCenter() - Theme::fontSmall.getWidth(getText()) / 2,
				bounds.getY() + _image->getSize().getHeight() + _textOffset
			),
			Theme::fontSmall,
			isActive() ? getActiveTextColor() : getDefaultTextColor(),
			getText()
		);
	}

	MenuView* MenuViewButton::getMenuView() const {
		return reinterpret_cast<MenuView*>(getParent());
	}
}
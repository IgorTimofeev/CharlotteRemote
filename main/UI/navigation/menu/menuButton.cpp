#include "menuButton.h"

#include "menuSection.h"
#include "menu.h"

#include "../../../rc.h"

namespace pizda {
	// -------------------------------- Default --------------------------------

	MenuButton::MenuButton(const Image* image, const std::wstring_view& text) : _image(image) {
		setSize(Size(45, image->getSize().getHeight() + _textOffset + Theme::fontSmall.getHeight()));
		setText(text);

		setDefaultBackgroundColor(&Theme::bg3);
		setPressedBackgroundColor(&Theme::bg3);

		setPressedBorderColor(&Theme::fg1);

		setDefaultTextColor(&Theme::fg4);
		setPressedTextColor(&Theme::fg1);
	}

	void MenuButton::onRender(Renderer* renderer, const Bounds& bounds) {
		constexpr static const uint8_t cornerRadius = 4;

		// Background
		renderer->renderFilledRectangle(
			Bounds(bounds.getX() - 1, bounds.getY() - 1, bounds.getWidth() + 2, _image->getSize().getHeight() + 2),
			cornerRadius,
			isChecked() ? getPressedBackgroundColor() : getDefaultBackgroundColor()
		);

		// Image
		renderer->renderImage(bounds.getTopLeft(), _image);

		// Border
		if (isChecked()) {
			renderer->renderRectangle(
				Bounds(bounds.getX() - 1, bounds.getY() - 1, bounds.getWidth() + 2, _image->getSize().getHeight() + 2),
				cornerRadius,
				getPressedBorderColor()
			);
		}

		// Text
		renderer->renderString(
			Point(
				bounds.getXCenter() - Theme::fontSmall.getWidth(getText()) / 2,
				bounds.getY() + _image->getSize().getHeight() + _textOffset
			),
			&Theme::fontSmall,
			isChecked() ? getPressedTextColor() : getDefaultTextColor(),
			getText()
		);
	}

	MenuSection* MenuButton::getMenuSection() {
		return reinterpret_cast<MenuSection*>(getParent()->getParent());
	}
}
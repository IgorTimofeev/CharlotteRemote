#include "menuButton.h"

#include "menuView.h"
#include "menu.h"

#include "../../../rc.h"

namespace pizda {
	// -------------------------------- Default --------------------------------

	MenuButton::MenuButton(const Image* image, const std::wstring_view& text) : _image(image) {
		setSize(Size(45, 45));
		setText(text);

		setDefaultBackgroundColor(&Theme::fg5);
		setPressedBackgroundColor(&Theme::fg1);
	}

	void MenuButton::onRender(Renderer* renderer, const Bounds& bounds) {
		constexpr static const uint8_t textHeight = 45 - 33;
		constexpr static const uint8_t cornerRadius = 3;
		constexpr static const uint8_t rectangleHeight = textHeight + cornerRadius;

		const int32_t y2 = bounds.getY2();
		const int32_t rectangleY = y2 - rectangleHeight + 1;
		const auto color = isChecked() ? getPressedBackgroundColor() : getDefaultBackgroundColor();

		renderer->renderRectangle(
			Bounds(bounds.getX(), rectangleY, bounds.getWidth(), rectangleHeight),
			cornerRadius,
			color
		);

		// Image
		renderer->renderImage(bounds.getTopLeft(), _image);

		// Small lines over image
		renderer->renderVerticalLine(Point(bounds.getX(), rectangleY), cornerRadius, color);
		renderer->renderVerticalLine(Point(bounds.getX2(), rectangleY), cornerRadius, color);

		// Text
		renderer->renderString(
			Point(
				bounds.getXCenter() - Theme::fontSmall.getWidth(getText()) / 2,
				y2 - textHeight + 1 + 2
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
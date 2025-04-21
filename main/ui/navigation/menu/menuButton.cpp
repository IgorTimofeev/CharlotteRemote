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
		if (isChecked()) {
			// Background
			renderer->renderRectangle(bounds, 3, getPressedBorderColor());
		}
		else {
			// Background
			renderer->renderRectangle(bounds, 3, getDefaultBorderColor());
		}

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
		return dynamic_cast<MenuView*>(getParent()->getParent());
	}

	Menu* MenuButton::getMenu() {
		return dynamic_cast<Menu*>(getMenuView()->getParent()->getParent());
	}

	// -------------------------------- Route --------------------------------

	RouteMenuButton::RouteMenuButton(const std::wstring_view& text, const Route* route) : MenuButton(text), _route(route) {
		setChecked(RC::getInstance().getRoute() == route);
	}

	void RouteMenuButton::onClick() {
		Button::onClick();

		RC::getInstance().getApplication().enqueueOnTick([this]() {
			auto& rc = RC::getInstance();
			rc.setRoute(_route);
			rc.hideMenu();
		});
	}

	// -------------------------------- View --------------------------------

	ViewMenuButton::ViewMenuButton(const std::wstring_view& text, const Route* route) : MenuButton(text), _route(route) {

	}

	void ViewMenuButton::onClick() {
		Button::onClick();

		RC::getInstance().getApplication().enqueueOnTick([this]() {
			getMenu()->setView(_route);
		});
	}

	// -------------------------------- Option --------------------------------

	OptionMenuButton::OptionMenuButton(const std::wstring_view& text) : MenuButton(text) {
		setToggle(true);
		setPressedBorderColor(&Theme::fg1);
	}
}
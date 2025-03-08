#include "menuItem.h"

#include "menu.h"
#include "../../rc.h"

namespace pizda {
	MenuItem::MenuItem(std::wstring_view text) {
		setText(text);
	}

	TitleMenuItem::TitleMenuItem(std::wstring_view text) : MenuItem(text) {
		setEnabled(false);
		setHeight(28);
	}

	void TitleMenuItem::onRender(Renderer* renderer, const Bounds& bounds) {
		constexpr static const uint8_t textOffset = 10;

		renderer->renderString(
			Point(
				bounds.getX() + textOffset,
				bounds.getYCenter() - Theme::fontNormal.getHeight() / 2
			),
			&Theme::fontNormal,
			&Theme::fg1,
			getText()
		);
	}

	PageMenuItem::PageMenuItem(std::wstring_view text, const Route* route) : MenuItem(text), _route(route) {
		setHeight(28);
	}

	void PageMenuItem::onRender(Renderer* renderer, const Bounds& bounds) {
		if (isSelected()) {
			// Background
			renderer->renderFilledRectangle(bounds, &Theme::bg3);

			// Line
			renderer->renderVerticalLine(
				bounds.getTopLeft(),
				bounds.getHeight(),
				&Theme::accent1
			);
		}

		// Text
		constexpr static const uint8_t textOffset = 15;

		renderer->renderString(
			Point(
				bounds.getX() + textOffset,
				bounds.getYCenter() - Theme::fontNormal.getHeight() / 2
			),
			&Theme::fontNormal,
			isSelected() ? &Theme::fg1 : &Theme::fg3,
			getText()
		);
	}

	const Route* PageMenuItem::getRoute() const {
		return _route;
	}

	void PageMenuItem::onEvent(Event* event) {
		Element::onEvent(event);

		if (event->getTypeID() != TouchDownEvent::typeID)
			return;

		auto menu = reinterpret_cast<Menu*>(getParent()->getParent()->getParent()->getParent());
		auto& rc = RC::getInstance();

		rc.setRoute(_route);

		menu->updateItemsSelection();

		event->setHandled(true);

		rc.setMenuVisibility(false);
	}
}
#include "menuItem.h"

#include "menu.h"
#include "../../rc.h"

namespace pizda {
	MenuItem::MenuItem(std::wstring_view text) {
		setText(text);
	}

	TitleMenuItem::TitleMenuItem(std::wstring_view text) : MenuItem(text) {
		setEnabled(false);
		setHeight(25);
	}

	void TitleMenuItem::onRender(Renderer* renderer, const Bounds& bounds) {
		constexpr static const uint8_t textOffset = 12;

		const auto textPosition = Point(
			bounds.getX() + textOffset,
			bounds.getYCenter() - Theme::fontNormal.getHeight() / 2
		);

		renderer->renderString(
			textPosition,
			&Theme::fontNormal,
			&Theme::fg7,
			getText()
		);

		constexpr static const uint8_t spacing = 8;

		const int32_t lineX = textPosition.getX() + Theme::fontNormal.getWidth(getText()) + spacing;
		const int32_t lineWidth = bounds.getX2() - lineX - spacing;

		if (lineWidth > 0) {
			renderer->renderHorizontalLine(
				Point(lineX, bounds.getYCenter()),
				lineWidth,
				&Theme::fg7
			);
		}
	}

	PageMenuItem::PageMenuItem(std::wstring_view text, const Route* route) : MenuItem(text), _route(route) {
		setHeight(25);
	}

	void PageMenuItem::onRender(Renderer* renderer, const Bounds& bounds) {
		if (isSelected()) {
			// Background
			renderer->renderFilledRectangle(bounds, &Theme::bg3);

			// Line
			renderer->renderFilledRectangle(
				Bounds(
					bounds.getX(),
					bounds.getY(),
					2,
					bounds.getHeight()
				),
				&Theme::accent1
			);
		}

		// Text
		constexpr static const uint8_t textOffset = 20;

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

		auto& rc = RC::getInstance();

		rc.setRoute(_route);

		auto menu = reinterpret_cast<Menu*>(getParent()->getParent()->getParent());
		menu->updatePageItemsSelection();

		rc.setMenuVisibility(false);

		event->setHandled(true);
	}

	void PageMenuItem::updateSelection() {
		setSelected(RC::getInstance().getRoute() == _route);
	}
}
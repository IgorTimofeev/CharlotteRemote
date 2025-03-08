#include "menuItem.h"

#include "menu.h"
#include "../../rc.h"

namespace pizda {
	MenuItem::MenuItem(std::wstring_view text) {
		setText(text);
	}

	TitleMenuItem::TitleMenuItem(std::wstring_view text) : MenuItem(text) {
		setEnabled(false);
		setHeight(10);
	}

	void TitleMenuItem::onRender(Renderer* renderer, const Bounds& bounds) {
		renderer->renderString(
			Point(
				bounds.getX(),
				bounds.getYCenter() - Theme::fontNormal.getHeight() / 2
			),
			&Theme::fontNormal,
			&Theme::fg4,
			getText()
		);
	}

	PageMenuItem::PageMenuItem(std::wstring_view text, const std::function<Page*()>& pageBuilder) : MenuItem(text), _pageBuilder(pageBuilder) {
		setHeight(30);
	}

	const std::function<Page*()>& PageMenuItem::getPageBuilder() const {
		return _pageBuilder;
	}

	void PageMenuItem::onRender(Renderer* renderer, const Bounds& bounds) {
		if (isSelected()) {
			// Background
			renderer->renderFilledRectangle(bounds, &Theme::bg3);

			// Line
			renderer->renderVerticalLine(
				bounds.getTopLeft(),
				bounds.getHeight(),
				&Theme::fg1
			);
		}

		// Text
		constexpr static const uint8_t textOffset = 10;

		renderer->renderString(
			Point(
				bounds.getX() + textOffset,
				bounds.getYCenter() - Theme::fontNormal.getHeight() / 2
			),
			&Theme::fontNormal,
			isSelected() ? &Theme::fg1 : &Theme::fg4,
			getText()
		);
	}
}
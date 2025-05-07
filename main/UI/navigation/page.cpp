#include "page.h"

namespace pizda {
	ScrollViewPage::ScrollViewPage() {
		// Title
		Theme::applyTitle(&title);
		title.setVerticalAlignment(Alignment::center);
		rows += &title;

		// Rows
		rows.setSpacing(10);
		rows.setMargin(Margin(15));
		scrollView += &rows;

		// Scroll views
		Theme::apply(&scrollView);
		*this += &scrollView;

		// Menu button
		menuButton.setSize(Size(36));
		menuButton.setMargin(Margin(0, 0, 0, -menuButton.getSize().getWidth() / 2 - 3));
		menuButton.setCornerRadius(menuButton.getSize().getWidth() / 2 - 1);
		menuButton.setContentMargin(Margin(0, 0, 0, 7));
		menuButton.setAlignment(Alignment::center, Alignment::end);

		menuButton.setDefaultBackgroundColor(&Theme::bg3);
		menuButton.setDefaultTextColor(&Theme::fg3);

		menuButton.setPressedBackgroundColor(&Theme::fg1);
		menuButton.setPressedTextColor(&Theme::bg1);

		menuButton.setFont(&Theme::fontSmall);
		menuButton.setText(L"^");

		*this += &menuButton;
	}
}
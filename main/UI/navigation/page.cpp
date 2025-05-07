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

		// Menu button
		menuButton.setSize(Size(Size::computed, 12));
		menuButton.setVerticalAlignment(Alignment::end);

		menuButton.setDefaultBackgroundColor(&Theme::bg2);
		menuButton.setDefaultTextColor(&Theme::fg5);

		menuButton.setPressedBackgroundColor(&Theme::fg1);
		menuButton.setPressedTextColor(&Theme::bg1);

		menuButton.setFont(&Theme::fontSmall);
		menuButton.setText(L"^");

		*this += &menuButton;

		Theme::apply(&scrollView);
		scrollView.setMargin(Margin(0, 0, 0, menuButton.getSize().getHeight()));
		*this += &scrollView;
	}
}
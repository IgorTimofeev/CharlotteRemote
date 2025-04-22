#include "page.h"

namespace pizda {
	ScrollViewPage::ScrollViewPage() {
		rows.setSpacing(10);
		rows.setMargin(Margin(15, 15));

		// Upper row
		upperRow.setOrientation(Orientation::horizontal);
		upperRow.setSpacing(10);
		upperRow.setMargin(Margin(0, 0, 0, 5));

		// Title
		Theme::applyTitle(&title);
		title.setVerticalAlignment(Alignment::center);
		upperRow += &title;

		// Menu button
		menuButton.setVerticalAlignment(Alignment::center);
		upperRow.setAutoSize(&menuButton);
		upperRow += &menuButton;

		rows += &upperRow;

		scrollView += &rows;

		Theme::apply(&scrollView);
		*this += &scrollView;
	}
}
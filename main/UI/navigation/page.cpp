#include "page.h"

namespace pizda {
	ScrollViewPage::ScrollViewPage() {
		// Title
		Theme::applyPageTitle(&title);
		title.setVerticalAlignment(Alignment::center);
		rows += &title;

		// Rows
		rows.setSpacing(Theme::spacing);
		// Bottom margin should be greater for nav menu button
		rows.setMargin(Margin(15, 15, 15, 25));
		scrollView += &rows;
		
		// Scroll views
		Theme::apply(&scrollView);
		*this += &scrollView;
	}
}
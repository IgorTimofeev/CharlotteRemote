#include "page.h"

#include <rc.h>

namespace pizda {
	ScrollViewPage::ScrollViewPage() {
		// Title
		Theme::applyPageTitle(&title);
		title.setVerticalAlignment(Alignment::center);
		rows += &title;

		// Rows
		rows.setSpacing(Theme::spacing);
		rows.setMargin(Margin(15));
		scrollView += &rows;

		// Scroll views
		Theme::apply(&scrollView);
		*this += &scrollView;

		RC::getInstance().getOpenMenuButton().applyBottomStyle();
	}
}
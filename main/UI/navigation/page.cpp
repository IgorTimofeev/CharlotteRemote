#include "page.h"

#include "../../rc.h"

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

		RC::getInstance().getOpenMenuButton().applyBottomStyle();
	}
}
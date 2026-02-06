#include "page.h"

namespace pizda {
	// ----------------------------- Templates -----------------------------

	void Page::setDefaultContentMargin(Element* element) const {
		element->setMargin(Margin(15, 15, 15, 20));
	}

	TitlePageTemplate::TitlePageTemplate() {
		// Title
		Theme::applyPageTitle(&title);
		title.setVerticalAlignment(Alignment::center);
	}

	StackRowsPageTemplate::StackRowsPageTemplate() {
		// Rows
		rows.setSpacing(Theme::spacing);
		setDefaultContentMargin(&rows);

		// Title
		rows += &title;
	}

	// ----------------------------- Ready to use -----------------------------

	ScrollViewPage::ScrollViewPage() {
		// Scroll view
		Theme::apply(&scrollView);
		*this += &scrollView;

		// Rows
		scrollView += &rows;
	}

	RelativeRowsPage::RelativeRowsPage() {
		// Rows
		rows.setSpacing(Theme::spacing);
		setDefaultContentMargin(&rows);
		*this += &rows;

		// Title
		rows += &title;
		rows.setAutoSize(&title);
	}
}

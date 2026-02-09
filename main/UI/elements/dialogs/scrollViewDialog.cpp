#include "scrollViewDialog.h"

#include "UI/theme.h"

namespace pizda {
	ScrollViewDialog::ScrollViewDialog() {
		// ScrollView
		Theme::apply(&scrollView);
		slideLayout += &scrollView;

		// Rows
		rows.setMargin(Margin(15));
		rows.setGap(Theme::gap);
		scrollView += &rows;

		// Title
		Theme::applyPageTitle(&title);
		rows += &title;
	}
}

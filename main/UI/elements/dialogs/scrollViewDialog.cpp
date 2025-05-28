#include "scrollViewDialog.h"

#include <UI/theme.h>

namespace pizda {
	ScrollViewDialog::ScrollViewDialog() {
		// Rows
		rows.setMargin(Margin(15));
		rows.setSpacing(Theme::spacing);

		// Title
		Theme::applyPageTitle(&title);
		rows += &title;

		// ScrollView
		Theme::apply(&scrollView);
		scrollView += &rows;
		slideLayout += &scrollView;
	}
}

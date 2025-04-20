#pragma once

#include "../../../components/yoba/src/main.h"
#include "../../../components/yoba/src/ui.h"
#include "../theme.h"

#include "openMenuButton.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class Page : public Layout {
		public:

	};

	class ScrollViewPage : public Page {
		public:
			ScrollViewPage() {
				rows.setSpacing(10);
				rows.setMargin(Margin(15, 15));

				// Upper row
				upperRow.setOrientation(Orientation::horizontal);
				upperRow.setSpacing(10);
				upperRow.setMargin(Margin(0, 0, 0, 5));

				// Menu button
				menuButton.setVerticalAlignment(Alignment::center);
				upperRow += &menuButton;

				// Title
				Theme::applyTitle(&title);
				title.setVerticalAlignment(Alignment::center);
				upperRow += &title;

				rows += &upperRow;

				scrollView += &rows;

				Theme::apply(&scrollView);
				*this += &scrollView;
			}

			ScrollView scrollView;
			StackLayout rows;

			StackLayout upperRow;
			OpenMenuButton menuButton;
			Text title;
	};
}
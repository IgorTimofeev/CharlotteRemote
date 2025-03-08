#pragma once

#include "../../../components/yoba/src/main.h"
#include "../../../components/yoba/src/ui.h"
#include "../theme.h"

#include "menuButton.h"

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
				rows.setMargin(Margin(20, 15));

				// Menu button
				upperRow += &menuButton;

				// Page title
				Theme::applyPageTitle(&title);
				upperRow += &title;

				// Upper row
				upperRow.setSpacing(10);
				rows += &upperRow;

				scrollView += &rows;

				Theme::apply(&scrollView);
				*this += &scrollView;
			}

			ScrollView scrollView;
			StackLayout rows;

			StackLayout upperRow;
			MenuButton menuButton;
			Text title;
	};
}
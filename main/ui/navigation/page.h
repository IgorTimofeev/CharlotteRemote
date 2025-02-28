#pragma once

#include "../../../components/yoba/src/main.h"
#include "../../../components/yoba/src/ui.h"
#include "../theme.h"

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
				rows.setMargin(Margin(20, 20, 20, 20));

				// Page title
				Theme::applyPageTitle(&pageTitle);
				rows += &pageTitle;

				scrollView += &rows;

				Theme::apply(&scrollView);
				*this += &scrollView;
			}

			ScrollView scrollView;
			StackLayout rows;
			Text pageTitle;
	};
}
#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include "UI/theme.h"

#include "menu/openMenuButton.h"

namespace pizda {
	using namespace YOBA;

	class Page : public Layout {
		public:
			void setDefaultContentMargin(Element* element) const;
	};

	// ----------------------------- Templates -----------------------------

	class TitlePageTemplate : public Page {
		public:
			TitlePageTemplate();

			TextView title {};
	};

	class StackRowsPageTemplate : public TitlePageTemplate {
		public:
			StackRowsPageTemplate();

			StackLayout rows {};
	};

	// ----------------------------- Ready to use -----------------------------

	class ScrollViewPage : public StackRowsPageTemplate {
		public:
			ScrollViewPage();

			ScrollView scrollView {};
	};

	class RelativeRowsPage : public TitlePageTemplate {
		public:
			RelativeRowsPage();

			RelativeStackLayout rows {};
	};
}
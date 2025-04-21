#pragma once

#include "../../../components/yoba/src/main.h"
#include "../../../components/yoba/src/ui.h"
#include "../theme.h"

#include "menu/openMenuButton.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class Page : public Layout {
		public:

	};

	class ScrollViewPage : public Page {
		public:
			ScrollViewPage();

			ScrollView scrollView;
			StackLayout rows;

			RelativeStackLayout upperRow;
			OpenMenuButton menuButton {};
			Text title;
	};
}
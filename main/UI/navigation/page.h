#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include "../theme.h"

#include "menu/openMenuButton.h"

namespace pizda {
	using namespace YOBA;

	class Page : public Layout {

	};

	class ScrollViewPage : public Page {
		public:
			ScrollViewPage();

			ScrollView scrollView {};
			StackLayout rows {};

			Text title {};
	};
}
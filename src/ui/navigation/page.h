#pragma once

#include "../../../lib/yoba/src/main.h"
#include "../../../lib/yoba/src/ui.h"

namespace pizdanc {
	using namespace yoba::ui;

	class Page : public Layout {
		public:
			virtual void setup() {

			}
	};

	class RowsPage : public Page {
		public:
			RowsPage() {
				rows.setSpacing(10);
				*this += &rows;
			}

			StackLayout rows = StackLayout();
	};
}
#pragma once

#include "../elements/rows.h"

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
				*this += &rows;
			}

			Rows rows = Rows();
	};
}
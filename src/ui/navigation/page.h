#pragma once

#include "../../../lib/YOBA/src/ui/layout.h"
#include "../elements/rows.h"

using namespace yoba;

namespace pizdanc {
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
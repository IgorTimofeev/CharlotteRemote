#pragma once

#include "../elements/rows.h"

using namespace yoba::ui;

namespace pizdanc {
	class Page : public Container {
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
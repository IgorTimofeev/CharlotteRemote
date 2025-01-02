#pragma once

#include "../../../lib/YOBA/src/ui/container.h"
#include "../elements/rows.h"

using namespace yoba;

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
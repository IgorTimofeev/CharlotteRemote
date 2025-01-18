#pragma once

#include "../../../lib/yoba/src/main.h"
#include "../../../lib/yoba/src/ui.h"

namespace pizdanc {
	using namespace yoba;
	using namespace yoba::ui;

	class Page : public Layout {
		public:
			virtual ~Page() {

			}

			virtual void setup() {

			}
	};
}
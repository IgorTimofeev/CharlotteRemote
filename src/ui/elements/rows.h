#pragma once

#include "../../../lib/yoba/src/ui.h"

namespace pizdanc {
	using namespace yoba;
	using namespace yoba::ui;

	class Rows : public StackLayout {
		public:
			Rows() {
				setSpacing(10);
			}
	};
}
#pragma once

#include "../../../lib/YOBA/src/ui.h"

using namespace yoba;
using namespace yoba::ui;

namespace pizdanc {
	class Rows : public StackLayout {
		public:
			Rows() {
				setSpacing(10);
			}
	};
}
#pragma once

#include "../../../lib/YOBA/src/ui/stackLayout.h"

using namespace yoba;

namespace pizdanc {
	class Rows : public StackLayout {
		public:
			Rows() {
				setSpacing(10);
			}
	};
}
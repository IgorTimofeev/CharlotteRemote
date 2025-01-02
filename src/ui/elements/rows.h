#pragma once

#include "../../../lib/YOBA/src/ui/stackContainer.h"

using namespace yoba;

namespace pizdanc {
	class Rows : public StackContainer {
		public:
			Rows() {
				setSpacing(10);
			}
	};
}
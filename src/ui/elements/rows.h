#pragma once

#include "yoba/elements/stack_layout.h"

using namespace yoba;

namespace pizdanc {
	class Rows : public StackLayout {
		public:
			Rows() {
				setSpacing(10);
			}
	};
}
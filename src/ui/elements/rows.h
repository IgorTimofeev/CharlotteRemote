#pragma once

#include "yoba/elements/stack_layout.h"

using namespace yoba;

namespace ui {
	class Rows : public StackLayout {
		public:
			Rows() {
				setSpacing(10);
			}
	};
}
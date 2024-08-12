#pragma once

#include "element.h"
#include "yoba/color.h"

namespace yoba {
	class ForegroundAware : public virtual Element {
		public:
			const Color* getForeground() const {
				return _foreground;
			}

			void setForeground(const Color* value) {
				_foreground = value;
			}

		private:
			const Color* _foreground = &HighColor::white;
	};
}

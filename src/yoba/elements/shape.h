#pragma once

#include "element.h"
#include "yoba/color.h"
#include "yoba/screen.h"

namespace yoba {
	class Shape : public Element {
		public:
			const Color* getFillColor() const {
				return _fillColor;
			}

			void setFillColor(const Color* value) {
				_fillColor = value;
			}

		private:
			const Color* _fillColor = &Color16::white;
	};
}

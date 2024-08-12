#pragma once

#include "shape.h"
#include "yoba/color.h"
#include "yoba/screen.h"

namespace yoba {
	class Rectangle : public Shape {
		public:
			Rectangle() = default;

			explicit Rectangle(const Color* color) {
				setFillColor(color);
			}

			void onRender(Screen& display) override {
				display.renderRectangle(getBounds(), getCornerRadius(), getFillColor());
			}

			// -------------------------------- Getters & setters --------------------------------

			uint8_t getCornerRadius() const {
				return _cornerRadius;
			}

			void setCornerRadius(uint8_t value) {
				_cornerRadius = value;
			}

		private:
			uint8_t _cornerRadius = 0;
	};
}

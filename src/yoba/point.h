#pragma once

#include "Arduino.h"
#include <cstdint>

namespace yoba {
	struct Point {
		public:
			Point() = default;
			Point(const Point& point) = default;

			Point(int32_t x, int32_t y) : x(x), y(y) {

			}

			int32_t getX() const {
				return x;
			}

			void setX(int32_t value) {
				x = value;
			}

			int32_t getY() const {
				return y;
			}

			void setY(int32_t value) {
				y = value;
			}

			bool operator==(const Point &right) const {
				return x == right.x && y == right.y;
			}

			bool operator!=(const Point &right) const {
				return !operator==(right);
			}

		private:
			int32_t x = 0;
			int32_t y = 0;
	};
}
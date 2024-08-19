#pragma once

#include "Arduino.h"
#include <cstdint>

namespace yoba {
	struct Point {
		public:
			Point() = default;
			Point(const Point& point) = default;

			Point(int32_t x, int32_t y) : _x(x), _y(y) {

			}

			int32_t getX() const {
				return _x;
			}

			void setX(int32_t value) {
				_x = value;
			}

			int32_t getY() const {
				return _y;
			}

			void setY(int32_t value) {
				_y = value;
			}

			Point rotate(float angle) const {
				auto angleSin = sin(angle);
				auto angleCos = cos(angle);

				return {
					(int32_t) ((float) _x * angleCos - (float) _y * angleSin),
					(int32_t) ((float) _x * angleSin - (float) _y * angleCos)
				};
			}

			bool operator==(const Point &right) const {
				return _x == right._x && _y == right._y;
			}

			bool operator!=(const Point &right) const {
				return !operator==(right);
			}

		private:
			int32_t _x = 0;
			int32_t _y = 0;
	};
}
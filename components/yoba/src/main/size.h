#pragma once

#include <cstdint>
#include "vector.h"

namespace yoba {
	struct Size {
		public:
			static const uint16_t computed = 0xFFFF;
			static const uint16_t infinity = 0xFFFE;

			Size();
			Size(uint16_t width, uint16_t height);
			Size(const Size& size) = default;

			uint16_t getWidth() const;
			void setWidth(uint16_t value);

			uint16_t getHeight() const;
			void setHeight(uint16_t value);

			uint16_t getXCenter() const;
			uint16_t getYCenter() const;
			Point getCenter() const;
			uint32_t getSquare() const;

			Size interpolate(const Size& to, const double &position) const;

			bool operator==(const Size& rhs) const;
			bool operator!=(const Size& rhs) const;

		private:
			uint16_t _width;
			uint16_t _height;
	};
}
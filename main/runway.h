#pragma once

#include <cstdint>
#include <cmath>

namespace pizda {
	class Runway {
		public:
			Runway(const GeographicCoordinates& center, uint16_t headingDegrees, uint16_t length, uint16_t width, const std::wstring_view& name) :
				_center(center),
				_headingDegrees(headingDegrees),
				_length(length),
				_width(width)
			{
				std::copy(name.begin(), name.end(), _name);
			}

			const GeographicCoordinates& getCenter() const {
				return _center;
			}

			uint16_t getHeadingDegrees() const {
				return _headingDegrees;
			}

			uint16_t getLength() const {
				return _length;
			}

			uint16_t getWidth() const {
				return _width;
			}

			const wchar_t* getName() const {
				return _name;
			}

		private:
			GeographicCoordinates _center;
			uint16_t _headingDegrees;
			uint16_t _length;
			uint16_t _width;
			wchar_t _name[5];
	};
}
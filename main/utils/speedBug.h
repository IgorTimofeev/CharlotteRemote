#pragma once

#include <string_view>
#include <cstdint>

namespace pizda {
	class SpeedBug {
		public:
			constexpr SpeedBug(std::wstring_view name, const uint16_t value) : _name(name), _value(value) {

			}

			std::wstring_view getName() const {
				return _name;
			}
			
			uint16_t getValue() const {
				return _value;
			}

		private:
			const std::wstring_view _name;
			const uint16_t _value;
	};
}
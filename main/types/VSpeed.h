#pragma once

#include <string_view>
#include <cstdint>

namespace pizda {
	class VSpeed {
		public:
			constexpr VSpeed(std::wstring_view name, const uint32_t value) : _name(name), _value(value) {

			}

			std::wstring_view getName() const {
				return _name;
			}

			uint32_t getValue() const {
				return _value;
			}

		private:
			const std::wstring_view _name;
			const uint32_t _value;
	};
}
#pragma once

#include <ranges>
#include <locale>
#include <cstdlib>
#include <string>

namespace pizda {
	class StringUtils {
		public:
			static bool containsIgnoreCase(std::string_view haystack, std::string_view needle) {
				return !std::ranges::search(
					haystack,
					needle,
					[](const char a, const char b) {
						return std::towlower(a) == std::towlower(b);
					}
				).empty();
			}

			// Just for demo
			static std::wstring toWString(const float value) {
				auto result = std::to_wstring(value);

				// I love C++

				// Erasing trailing zeros
				result.erase(result.find_last_not_of('0') + 1);

				// If the last character is a decimal point, removing it
				if (result.back() == '.')
					result.pop_back();

				return result;
			}
			
			static bool tryParseInt32(const std::string_view& source, int32_t& result) {
				char* endPtr;
				result = std::strtol(source.data(), &endPtr, 10);
				
				return endPtr != source.data();
			}

			static bool tryParseFloat(const std::string_view& source, float& result) {
				char* endPtr;
				result = std::strtof(source.data(), &endPtr);

				return endPtr != source.data();
			}

			static int32_t tryParseInt32Or(const std::string_view& source, const int32_t fallbackValue = 0) {
				int32_t result = 0;

				if (!tryParseInt32(source, result))
					result = fallbackValue;

				return result;
			}

			static float tryParseFloatOr(const std::string_view& source, const float fallbackValue = 0) {
				float result = 0;

				if (!tryParseFloat(source, result))
					result = fallbackValue;

				return result;
			}
	};
}
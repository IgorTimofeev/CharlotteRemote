#pragma once

#include <ranges>
#include <locale>
#include <codecvt>
#include <cstdlib>

namespace pizda {
	class StringUtils {
		public:
			static bool containsIgnoreCase(std::wstring_view haystack, std::wstring_view needle) {
				return !std::ranges::search(
					haystack,
					needle,
					[](const wchar_t a, const wchar_t b) {
						return std::towlower(a) == std::towlower(b);
					}
				).empty();
			}

			static std::wstring toWString(const std::string& source) {
				std::wstring_convert<std::codecvt_utf8<wchar_t>> converterX;

				return converterX.from_bytes(source.data(), source.data() + source.size());
			}

			static std::string toString(const std::wstring_view& source) {
				std::wstring_convert<std::codecvt_utf8<wchar_t>> converterX;

				return converterX.to_bytes(source.data(), source.data() + source.size());
			}

			static std::wstring toWString(const float value) {
				auto result = std::to_wstring(value);

				// I love C++

				// Erasing trailing zeros
				result.erase(result.find_last_not_of(L'0') + 1);

				// If the last character is a decimal point, removing it
				if (result.back() == L'.')
					result.pop_back();

				return result;
			}
			
			static bool tryParseInt32(const std::wstring_view& source, int32_t& result) {
				wchar_t* endPtr;
				result = std::wcstol(source.data(), &endPtr, 10);
				
				return endPtr != source.data();
			}

			static bool tryParseFloat(const std::wstring_view& source, float& result) {
				wchar_t* endPtr;
				result = std::wcstof(source.data(), &endPtr);

				return endPtr != source.data();
			}

			static int32_t tryParseInt32Or(const std::wstring_view& source, const int32_t fallbackValue = 0) {
				int32_t result = 0;

				if (!tryParseInt32(source, result))
					result = fallbackValue;

				return result;
			}

			static float tryParseFloatOr(const std::wstring_view& source, const float fallbackValue = 0) {
				float result = 0;

				if (!tryParseFloat(source, result))
					result = fallbackValue;

				return result;
			}
	};
}
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
			
			static bool tryParseInt32(const std::wstring_view& source, int32_t& result) {
				wchar_t* endPtr;
				result = std::wcstol(source.data(), &endPtr, 10);
				
				return endPtr != source.data();
			}
	};
}
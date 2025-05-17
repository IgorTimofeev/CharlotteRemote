#pragma once

#include <ranges>

namespace pizda {
	class StringUtils {
		public:
			static bool containsIgnoreCase(std::wstring_view haystack, std::wstring_view needle) {
				return !std::ranges::search(
					haystack,
					needle,
					[](wchar_t a, wchar_t b) {
						return std::towlower(a) == std::towlower(b);
					}
				).empty();
			}
	};
}
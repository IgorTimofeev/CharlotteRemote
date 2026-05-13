#pragma once

#include <ranges>
#include <locale>
// #include <codecvt>
#include <cstdlib>
#include <string>
#include <cwchar>

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

			// static std::wstring toWString(const std::string& source) {
			// 	std::wstring_convert<std::codecvt_utf8<wchar_t>> converterX;
			//
			// 	return converterX.from_bytes(source.data(), source.data() + source.size());
			// }
			//
			// static std::string toString(const std::wstring_view& source) {
			// 	std::wstring_convert<std::codecvt_utf8<wchar_t>> converterX;
			//
			// 	return converterX.to_bytes(source.data(), source.data() + source.size());
			// }

			static std::wstring toWString(const std::string& str) {
				if (str.empty())
					return L"";

				std::mbstate_t state {};
				const char* src = str.data();

				// Вычисляем необходимый размер буфера (включая null-терминатор)
				const size_t length = std::mbrtowc(nullptr, src, 0, &state);

				if (length == static_cast<size_t>(-1))
					return L"";

				std::wstring result(length, L'\0');
				src = str.data();
				std::mbrtowc(result.data(), src, str.size(), &state);

				return result;
			}

			static std::string toString(const std::wstring& wstr) {
				if (wstr.empty())
					return "";

				std::mbstate_t state {};
				const wchar_t* src = wstr.data();

				// Вычисляем необходимый размер буфера
				const size_t length = std::wcsrtombs(nullptr, &src, 0, &state);

				if (length == static_cast<size_t>(-1))
					return "";

				std::string result(length, '\0');
				src = wstr.data();
				std::wcsrtombs(result.data(), &src, result.size(), &state);

				return result;
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
#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include <YOBA/resources/fonts/PIXY10Font.h>
#include <YOBA/resources/fonts/Vaticanus8Font.h>
#include <YOBA/resources/fonts/unscii16.h>

namespace pizda {
	using namespace YOBA;

	class Theme {
		public:
			constexpr static Bit8PaletteColor bg1 { 0 };
			constexpr static Bit8PaletteColor bg2 { 1 };
			constexpr static Bit8PaletteColor bg3 { 2 };
			constexpr static Bit8PaletteColor bg4 { 3 };
			constexpr static Bit8PaletteColor bg5 { 4 };
			constexpr static Bit8PaletteColor bg6 { 5 };
			constexpr static Bit8PaletteColor bg7 { 6 };

			constexpr static Bit8PaletteColor fg1 { 7 };
			constexpr static Bit8PaletteColor fg2 { 8 };
			constexpr static Bit8PaletteColor fg3 { 9 };
			constexpr static Bit8PaletteColor fg4 { 10 };
			constexpr static Bit8PaletteColor fg5 { 11 };
			constexpr static Bit8PaletteColor fg6 { 12 };
			constexpr static Bit8PaletteColor fg7 { 13 };

			constexpr static Bit8PaletteColor accent1 { 14 };
			constexpr static Bit8PaletteColor accent2 { 15 };

			constexpr static Bit8PaletteColor good1 { 16 };
			constexpr static Bit8PaletteColor good2 { 17 };

			constexpr static Bit8PaletteColor bad1 { 18 };
			constexpr static Bit8PaletteColor bad2 { 19 };

			constexpr static Bit8PaletteColor red { 20 };
			constexpr static Bit8PaletteColor purple { 21 };
			constexpr static Bit8PaletteColor green { 22 };
			constexpr static Bit8PaletteColor greenSpeed { 23 };
			constexpr static Bit8PaletteColor yellow { 24 };
			constexpr static Bit8PaletteColor ocean { 25 };
			constexpr static Bit8PaletteColor ground { 26 };
			constexpr static Bit8PaletteColor ground2 { 27 };
			constexpr static Bit8PaletteColor sky { 28 };
			constexpr static Bit8PaletteColor sky2 { 29 };

			constexpr static PIXY10Font fontNormal {};
			constexpr static Vaticanus8Font fontSmall {};

			constexpr static uint16_t elementHeight = 27;
			constexpr static uint16_t cornerRadius = 3;
			constexpr static uint16_t spacing = 9;

			static void setup(Bit8PaletteRenderer* renderer);
			static void applyPrimary(Button* button);
			static void applySecondary(Button* button);
			static void applyCritical(Button* button);
			static void applyPlaceholder(Button* button);
			static void apply(Slider* slider);
			static void apply(TextField* textField);
			static void apply(ScrollView* scrollView);
			static void apply(Switch* sw);
			static void apply(ProgressBar* progressBar);
			static void apply(Text* text);
			static void applyPageTitle(Text* text);
			static void applyTitle(Text* text);
			static void applyDescription(Text* text);
	};
}
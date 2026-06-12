#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include <YOBA/resources/fonts/PIXY10Font.h>
#include <YOBA/resources/fonts/Vaticanus8Font.h>
#include <YOBA/resources/fonts/unscii16Font.h>

namespace pizda {
	using namespace YOBA;

	class Theme {
		public:
			// -------------------------------- Colors --------------------------------

			// Background
			constexpr static RGB565Color bg1 = RGB888Color(0x000000).toRGB565();
			constexpr static RGB565Color bg2 = RGB888Color(0x0C0C0C).toRGB565();
			constexpr static RGB565Color bg3 = RGB888Color(0x1C1C1C).toRGB565();
			constexpr static RGB565Color bg4 = RGB888Color(0x2C2C2C).toRGB565();
			constexpr static RGB565Color bg5 = RGB888Color(0x3C3C3C).toRGB565();
			constexpr static RGB565Color bg6 = RGB888Color(0x4C4C4C).toRGB565();
			constexpr static RGB565Color bg7 = RGB888Color(0x5C5C5C).toRGB565();
			// Foreground
			constexpr static RGB565Color fg1 = RGB888Color(0xF0F0F0).toRGB565();
			constexpr static RGB565Color fg2 = RGB888Color(0xCCCCCC).toRGB565();
			constexpr static RGB565Color fg3 = RGB888Color(0x999999).toRGB565();
			constexpr static RGB565Color fg4 = RGB888Color(0x888888).toRGB565();
			constexpr static RGB565Color fg5 = RGB888Color(0x777777).toRGB565();
			constexpr static RGB565Color fg6 = RGB888Color(0x666666).toRGB565();
			constexpr static RGB565Color fg7 = RGB888Color(0x555555).toRGB565();
			// Accent
			constexpr static RGB565Color accent1 = RGB888Color(0xffd200).toRGB565();
			constexpr static RGB565Color accent2 = RGB888Color(0x997e53).toRGB565();
			// Good
			constexpr static RGB565Color good1 = RGB888Color(0x1db153).toRGB565();
			constexpr static RGB565Color good2 = RGB888Color(0x28DA69).toRGB565();
			// Bad
			constexpr static RGB565Color bad1 = RGB888Color(0xf24949).toRGB565();
			constexpr static RGB565Color bad2 = RGB888Color(0x962828).toRGB565();
			constexpr static RGB565Color bad3 = RGB888Color(0x4b1313).toRGB565();
			// Red
			constexpr static RGB565Color red = RGB888Color(0xff0000).toRGB565();
			// Green
			constexpr static RGB565Color green1 = RGB888Color(0x00FF00).toRGB565();
			constexpr static RGB565Color green2 = RGB888Color(0x007F00).toRGB565();
			// Blue
			constexpr static RGB565Color blue = RGB888Color(0x0000FF).toRGB565();
			// Magenta
			constexpr static RGB565Color magenta1 = RGB888Color(0xFF00FF).toRGB565();
			constexpr static RGB565Color magenta2 = RGB888Color(0xAA00AA).toRGB565();
			// Ocean
			constexpr static RGB565Color ocean = RGB888Color(0x00FFFF).toRGB565();
			// Yellow
			constexpr static RGB565Color yellow = RGB888Color(0xffd200).toRGB565();
			// Ground
			constexpr static RGB565Color ground1 = RGB888Color(0x97b838).toRGB565();
			constexpr static RGB565Color ground2 = RGB888Color(0xdef2a2).toRGB565();
			// Sky
			constexpr static RGB565Color sky1 = RGB888Color(0x317fcb).toRGB565();
			constexpr static RGB565Color sky2 = RGB888Color(0xaed1f2).toRGB565();

			// -------------------------------- Fonts --------------------------------

			constexpr static PIXY10Font fontNormal {};
			constexpr static Vaticanus8Font fontSmall {};

			// -------------------------------- UI elements --------------------------------

			constexpr static uint16_t elementHeight = 27;
			constexpr static uint16_t cornerRadius = 3;
			constexpr static uint16_t verticalGap = 9;
			constexpr static uint16_t horizontalGap = 7;

			static void setup();
			static void applyPrimary(Button* button);
			static void applySecondary(Button* button);
			static void applyCritical(Button* button);
			static void applyPlaceholder(Button* button);
			static void apply(Slider* slider);
			static void apply(TextField* textField);
			static void apply(ScrollView* scrollView);
			static void apply(Switch* sw);
			static void apply(ProgressBar* progressBar);
			static void apply(TextView* text);
			static void applyPageTitle(TextView* text);
			static void applyTitle(TextView* text);
			static void applyDescription(TextView* text);
	};
}
#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include <YOBA/resources/fonts/PIXY10Font.h>
#include <YOBA/resources/fonts/Vaticanus8Font.h>

namespace pizda {
	using namespace YOBA;

	class Theme {
		public:
			static const PaletteColor bg1;
			static const PaletteColor bg2;
			static const PaletteColor bg3;
			static const PaletteColor bg4;
			static const PaletteColor bg5;
			static const PaletteColor bg6;
			static const PaletteColor bg7;

			static const PaletteColor fg1;
			static const PaletteColor fg2;
			static const PaletteColor fg3;
			static const PaletteColor fg4;
			static const PaletteColor fg5;
			static const PaletteColor fg6;
			static const PaletteColor fg7;

			static const PaletteColor accent1;
			static const PaletteColor accent2;

			static const PaletteColor good1;
			static const PaletteColor good2;

			static const PaletteColor bad1;
			static const PaletteColor bad2;

			static const PaletteColor red;
			static const PaletteColor purple;
			static const PaletteColor green;
			static const PaletteColor greenSpeed;
			static const PaletteColor yellow;
			static const PaletteColor blue;
			static const PaletteColor ground;
			static const PaletteColor ground2;
			static const PaletteColor sky;
			static const PaletteColor sky2;

			static const PIXY10Font fontNormal;
			static const Vaticanus8Font fontSmall;

			static const uint16_t elementHeight = 28;
			static const uint16_t cornerRadius = 3;

			static void setup(EightBitPaletteRenderer* renderer);
			static void apply(Button* button);
			static void apply(Slider* slider);
			static void apply(TextField* textField);
			static void apply(ScrollView* scrollView);
			static void apply(Switch* sw);
			static void apply(ProgressBar* progressBar);
			static void apply(Text* text);
			static void applyTitle(Text* text);
			static void applyDescription(Text* text);
	};
}
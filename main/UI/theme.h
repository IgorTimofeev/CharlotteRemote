#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include <YOBA/resources/fonts/PIXY10Font.h>
#include <YOBA/resources/fonts/Vaticanus8Font.h>

namespace pizda {
	using namespace YOBA;

	class Theme {
		public:
			static const Rgb565Color bg1;
			static const Rgb565Color bg2;
			static const Rgb565Color bg3;
			static const Rgb565Color bg4;
			static const Rgb565Color bg5;
			static const Rgb565Color bg6;
			static const Rgb565Color bg7;

			static const Rgb565Color fg1;
			static const Rgb565Color fg2;
			static const Rgb565Color fg3;
			static const Rgb565Color fg4;
			static const Rgb565Color fg5;
			static const Rgb565Color fg6;
			static const Rgb565Color fg7;

			static const Rgb565Color accent1;
			static const Rgb565Color accent2;

			static const Rgb565Color good1;
			static const Rgb565Color good2;

			static const Rgb565Color bad1;
			static const Rgb565Color bad2;

			static const Rgb565Color red;
			static const Rgb565Color purple;
			static const Rgb565Color green;
			static const Rgb565Color greenSpeed;
			static const Rgb565Color yellow;
			static const Rgb565Color blue;
			static const Rgb565Color ground;
			static const Rgb565Color ground2;
			static const Rgb565Color sky;
			static const Rgb565Color sky2;

			static const PIXY10Font fontNormal;
			static const Vaticanus8Font fontSmall;

			static const uint16_t elementHeight = 28;
			static const uint16_t cornerRadius = 3;

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
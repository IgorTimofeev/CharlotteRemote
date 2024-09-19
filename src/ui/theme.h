#pragma once

#include "../../lib/YOBA/src/color.h"
#include "../../lib/YOBA/src/resources/fonts/PIXY10Font.h"
#include "../../lib/YOBA/src/ui/button.h"
#include "../../lib/YOBA/src/ui/slider.h"

using namespace yoba;

namespace pizdanc {
	class Theme {
		public:
			static const Bit8PaletteColor bg1;
			static const Bit8PaletteColor bg2;
			static const Bit8PaletteColor bg3;
			static const Bit8PaletteColor bg4;
			static const Bit8PaletteColor bg5;

			static const Bit8PaletteColor fg1;
			static const Bit8PaletteColor fg2;
			static const Bit8PaletteColor fg3;
			static const Bit8PaletteColor fg4;

			static const Bit8PaletteColor red;
			static const Bit8PaletteColor purple;
			static const Bit8PaletteColor green;
			static const Bit8PaletteColor greenSpeed;
			static const Bit8PaletteColor yellow;
			static const Bit8PaletteColor blue;
			static const Bit8PaletteColor ground;
			static const Bit8PaletteColor sky;

			static const PIXY10Font font;

			static const uint16_t elementHeight = 32;
			static const uint16_t cornerRadius = 5;

			static void apply(Button& button);
			static void apply(Slider& slider);
	};
}
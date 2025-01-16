#pragma once

#include "color.h"
#include "../../lib/yoba/src/ui.h"
#include "../../lib/yoba/src/resources/fonts/PIXY10Font.h"
#include "../../lib/yoba/src/resources/fonts/Vaticanus8Font.h"
#include "../../lib/yoba/src/resources/fonts/unscii16Font.h"

namespace pizdanc {
	using namespace yoba;
	using namespace yoba::ui;

	class Theme {
		public:
			static const PaletteColor bg1;
			static const PaletteColor bg2;
			static const PaletteColor bg3;
			static const PaletteColor bg4;
			static const PaletteColor bg5;

			static const PaletteColor fg1;
			static const PaletteColor fg2;
			static const PaletteColor fg3;
			static const PaletteColor fg4;

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
//			static const unscii16Font fontBig;

			static const uint16_t elementHeight = 32;
			static const uint16_t cornerRadius = 5;

			static void apply(Titler& titler);
			static void apply(Button& button);
			static void apply(Slider& slider);
			static void apply(TextField& textField);
	};
}
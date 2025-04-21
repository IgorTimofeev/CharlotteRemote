#pragma once

#include "src/main.h"
#include "src/ui.h"

#include "../menu/openMenuButton.h"
#include "../../titler.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class InstrumentsView : public Layout {
		public:
			InstrumentsView();

			constexpr static const uint8_t titleVerticalOffset = 1;
			constexpr static const uint8_t contentVerticalOffset = 4;
			constexpr static const uint16_t titleSpacing = titleVerticalOffset + contentVerticalOffset;

		protected:
			OpenMenuButton menuButton;

			RelativeStackLayout buttonAndRowLayout {};
			StackLayout row {};
			Rectangle titlesRect {};

			void applyTitlerStyle(Titler& titler);
	};
}
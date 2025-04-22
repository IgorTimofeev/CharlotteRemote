#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "../menu/openMenuButton.h"
#include "../../titler.h"

namespace pizda {
	using namespace YOBA;

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
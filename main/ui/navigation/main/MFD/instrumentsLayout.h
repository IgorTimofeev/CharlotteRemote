#pragma once

#include "src/main.h"
#include "src/ui.h"

#include "../../openMenuButton.h"
#include "../../../titler.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class InstrumentsLayout : public Layout {
		public:
			InstrumentsLayout();

			constexpr static const uint8_t titleVerticalOffset = 1;
			constexpr static const uint8_t contentVerticalOffset = 4;
			constexpr static const uint16_t titleSpacing = titleVerticalOffset + contentVerticalOffset;

		protected:
			OpenMenuButton _openMenuButton {};
			StackLayout _row {};
			Rectangle _titlesRect {};

			void applyTitlerStyle(Titler& titler);
	};
}
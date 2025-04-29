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

			constexpr static const uint8_t topPanelHeight = 8;
			constexpr static const uint8_t contentHorizontalMargin = 7;
			constexpr static const uint8_t contentVerticalMargin = 5;
			constexpr static const uint8_t contentPanelMarginTop = topPanelHeight + contentVerticalMargin;
			constexpr static const uint8_t titleVerticalOffset = 1;

		protected:
			OpenMenuButton menuButton {};
			RelativeStackLayout buttonAndRowLayout {};
			StackLayout row {};
			Rectangle topPanel {};
	};
}
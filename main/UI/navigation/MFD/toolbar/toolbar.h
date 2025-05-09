#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

namespace pizda {
	using namespace YOBA;

	class Toolbar : public Layout {
		public:
			Toolbar();

			constexpr static uint8_t topPanelHeight = 8;
			constexpr static uint8_t contentHorizontalMargin = 7;
			constexpr static uint8_t contentVerticalMargin = 5;
			constexpr static uint8_t contentPanelMarginTop = topPanelHeight + contentVerticalMargin;
			constexpr static uint8_t titleVerticalOffset = 1;

		protected:
			StackLayout row {};
			Rectangle topPanel {};
	};
}
#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

namespace pizda {
	using namespace YOBA;

	class Toolbar : public Layout {
		public:
			Toolbar();

			constexpr static uint8_t topPanelHeight = 7;
			constexpr static uint8_t contentHorizontalMargin = 8;

		protected:
			StackLayout row {};
			Rectangle topPanel {};
	};
}
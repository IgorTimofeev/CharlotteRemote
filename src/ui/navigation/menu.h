#pragma once

#include "Arduino.h"

#include "../../../lib/yoba/src/ui.h"

#include "menuItem.h"

namespace pizdanc {
	class Menu : public Selector {
		public:
			void setup();

		protected:
			void onSelectionChanged() override;

		private:
			static const uint16_t selectorHeight = 20;

			Rectangle _background = Rectangle(&Theme::bg1);
			Layout _pageLayout;

			Layout _itemsHolder;
			Rectangle _itemsBackground = Rectangle(&Theme::bg2);
			EqualStackLayout _itemsLayout = EqualStackLayout(Orientation::horizontal);
	};
}
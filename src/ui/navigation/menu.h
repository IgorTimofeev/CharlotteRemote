#pragma once

#include "Arduino.h"

#include "../../../lib/yoba/src/ui.h"

#include "menuItem.h"

namespace pizdanc {
	class Menu : public Selector {
		public:
			explicit Menu();

		protected:
			void onSelectionChanged() override;

		private:
			static const uint16_t selectorHeight = 20;

			Layout _pageLayout;

			Layout _itemsHolder;
			Rectangle _itemsBackground = Rectangle(&Theme::bg2);
			EqualStackLayout _itemsLayout = EqualStackLayout(Orientation::horizontal);
	};
}
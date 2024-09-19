#pragma once

#include "Arduino.h"
#include "../../../lib/YOBA/src/color.h"
#include "../../../lib/YOBA/src/ui/selectorItem.h"
#include "../../../lib/YOBA/src/ui/shapes/rectangle.h"
#include "../../../lib/YOBA/src/ui/text.h"
#include "../../../lib/YOBA/src/ui/stackLayout.h"
#include "ui/theme.h"

using namespace yoba;

namespace pizdanc {
	class SideBarItem : public SelectorItem {
		public:
			explicit SideBarItem(const wchar_t* text) {
				// Selection
				_selectionBackground.setBackground(&Theme::fg1);
				addChild(&_selectionBackground);

				// Text
				_text.setMargin(Margin(8, 0, 8, 0));
				_text.setHorizontalAlignment(Alignment::Center);
				_text.setVerticalAlignment(Alignment::Center);
				_text.setText(text);
				addChild(&_text);

				setSelected(false);
			}

			void setSelected(const bool &value) override {
				_selectionBackground.setVisible(value);
				_text.setForeground(value ? &Theme::bg1 : &Theme::fg1);
			}

		private:
			Rectangle _selectionBackground = Rectangle(&Theme::bg1);

			StackLayout _row = StackLayout();

			Text _text = Text();
	};
}
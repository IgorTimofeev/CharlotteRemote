#pragma once

#include "Arduino.h"
#include "../theme.h"
#include "../../../lib/YOBA/src/color.h"
#include "../../../lib/YOBA/src/ui/selectorItem.h"
#include "../../../lib/YOBA/src/ui/shapes/rectangle.h"
#include "../../../lib/YOBA/src/ui/text.h"
#include "../../../lib/YOBA/src/ui/stackLayout.h"

using namespace yoba;

namespace pizdanc {
	class MenuItem : public SelectorItem {
		public:
			explicit MenuItem(const wchar_t* text) {
				// Selection
				_selectionBackground.setBackgroundColor(&Theme::fg1);
				*this += &_selectionBackground;

				// Text
				_text.setMargin(Margin(8, 0, 8, 0));
				_text.setHorizontalAlignment(Alignment::Center);
				_text.setVerticalAlignment(Alignment::Center);
				_text.setText(text);
				*this += &_text;

				updateVisualsFromSelection();
			}

			void setSelected(bool value) override {
				SelectorItem::setSelected(value);

				updateVisualsFromSelection();
			}

		private:
			Rectangle _selectionBackground = Rectangle(&Theme::bg1);
			StackLayout _row = StackLayout();
			Text _text = Text();

			void updateVisualsFromSelection() {
				_selectionBackground.setVisible(isSelected());
				_text.setForegroundColor(isSelected() ? &Theme::bg1 : &Theme::fg1);
			}
	};
}
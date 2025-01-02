#pragma once

#include "Arduino.h"
#include "../theme.h"
#include "../../../lib/YOBA/src/color.h"
#include "../../../lib/YOBA/src/ui/selector.h"
#include "../../../lib/YOBA/src/ui/rectangle.h"
#include "../../../lib/YOBA/src/ui/text.h"
#include "../../../lib/YOBA/src/ui/stackContainer.h"

using namespace yoba;

namespace pizdanc {
	class MenuItem : public SelectorItem {
		public:
			explicit MenuItem(const std::u32string_view& text) {
				// Selection
				_selectionBackground.setPrimaryColor(&Theme::fg1);
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
			StackContainer _row = StackContainer();
			Text _text = Text();

			void updateVisualsFromSelection() {
				_selectionBackground.setVisible(isSelected());
				_text.setPrimaryColor(isSelected() ? &Theme::bg1 : &Theme::fg1);
			}
	};
}
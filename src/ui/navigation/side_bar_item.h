#pragma once

#include "Arduino.h"
#include "yoba/color.h"
#include "yoba/image_source.h"
#include "yoba/elements/selector_item.h"
#include "yoba/elements/rectangle.h"
#include "yoba/elements/image.h"
#include "yoba/elements/text.h"
#include "yoba/elements/stack_layout.h"
#include "ui/theme.h"

using namespace yoba;

namespace ui {
	class SideBarItem : public SelectorItem {
		public:
			explicit SideBarItem(const String& text) {
				// Selection
				_selectionBackground.setFillColor(&Theme::fg1);
				addChild(&_selectionBackground);

				// Text
				_text.setMargin(Margin(8, 0, 8, 0));
				_text.setHorizontalAlignment(Alignment::center);
				_text.setVerticalAlignment(Alignment::center);
				_text.setText(text);
				addChild(&_text);

				setSelected(false);
			}

			void setSelected(const bool &value) override {
				_selectionBackground.setVisible(value);
				_text.setForeground(value ? &Theme::bg1 : &Theme::fg1);
			}

		private:
			Rectangle _selectionBackground = Rectangle(&HighColor::black);

			StackLayout _row = StackLayout();

			Text _text = Text();
	};
}
#pragma once

#include "element.h"
#include "yoba/color.h"
#include "yoba/screen.h"
#include "yoba/bounds.h"
#include "text_aware.h"
#include "foreground_aware.h"
#include "font_aware.h"

namespace yoba {
	class Text : public TextAware, public FontAware, public ForegroundAware {
		public:
			Text() = default;

			explicit Text(const String& value) {
				setText(value);
			}

			Size onMeasure(Screen& display, const Size& availableSize) override {
				display.setFont(getFont());

				return display.measureText(getText());
			}

			void onRender(Screen& display) override {
//				display.drawRectangle(getBounds(), Color::gold);

				display.setFont(getFont());
				display.renderText(getBounds().getPosition(), getForeground(), getText());
			}
	};
}

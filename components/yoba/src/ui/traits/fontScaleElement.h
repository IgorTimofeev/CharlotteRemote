#pragma once

#include "ui/element.h"

namespace yoba::ui {
	class FontScaleElement : public virtual Element {
		public:
			uint8_t getFontScale() const {
				return _fontScale;
			}

			void setFontScale(uint8_t value) {
				if (value == _fontScale)
					return;

				_fontScale = value;

				invalidate();
			}

		private:
			uint8_t _fontScale = 1;
	};
}
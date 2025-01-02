#pragma once

#include <sstream>
#include "ui/element.h"
#include "ui/theme.h"

using namespace yoba;

namespace pizdanc {
	class ThrottleIndicator : public Element {
		public:
			void render(ScreenBuffer* screenBuffer) override {
				const auto& bounds = getBounds();
				const uint8_t lineOffset = 2;
				const uint8_t textOffset = 2;
				const auto textHeight = Theme::fontNormal.getHeight();
				const uint16_t frameHeight = bounds.getHeight() - textHeight - textOffset;

				// Frame
				screenBuffer->renderRectangle(
					Bounds(
						bounds.getX(),
						bounds.getY(),
						bounds.getWidth(),
						frameHeight
					),
					&Theme::bg5
				);

				// Value rect
				const auto valueHeight = (uint16_t) ((float) frameHeight * _value);
				const uint16_t valueY = bounds.getY() + frameHeight - valueHeight + 1;

				screenBuffer->renderFilledRectangle(
					Bounds(
						bounds.getX(),
						valueY,
						bounds.getWidth(),
						valueHeight
					),
					&Theme::fg1
				);

				// Line
				screenBuffer->renderHorizontalLine(
					Point(
						bounds.getX() - lineOffset,
						valueY
					),
					lineOffset + bounds.getWidth() + lineOffset,
					&Theme::green
				);

				// Text
				std::basic_stringstream<char32_t> textBuffer;
				textBuffer << (int32_t) (_value * 100);

				screenBuffer->renderText(
					Point(
						bounds.getX() + bounds.getWidth() / 2 - Theme::fontNormal.getWidth<char32_t>(textBuffer.str()) / 2 + 1,
						bounds.getY() + frameHeight + textOffset
					),
					&Theme::fontNormal,
					&Theme::green,
					textBuffer.str()
				);
			}

			float getValue() const {
				return _value;
			}

			void setValue(float value) {
				_value = value;
			}

		private:
			float _value = 0;
	};
}
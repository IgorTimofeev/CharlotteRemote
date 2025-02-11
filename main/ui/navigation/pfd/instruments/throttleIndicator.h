#pragma once

#include <sstream>
#include "../../../components/yoba/src/ui.h"
#include "../../../theme.h"

namespace pizda {
	using namespace yoba;

	class ThrottleIndicator : public Element {
		public:
			void onRender(Renderer* renderer, const Bounds& bounds) override {
				const uint8_t lineOffset = 2;
				const uint8_t textOffset = 2;
				const auto textHeight = Theme::fontNormal.getHeight();
				const uint16_t frameHeight = bounds.getHeight() - textHeight - textOffset;

				// Frame
				renderer->renderRectangle(
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

				renderer->renderFilledRectangle(
					Bounds(
						bounds.getX(),
						valueY,
						bounds.getWidth(),
						valueHeight
					),
					&Theme::fg1
				);

				// Line
				renderer->renderHorizontalLine(
					Point(
						bounds.getX() - lineOffset,
						valueY
					),
					lineOffset + bounds.getWidth() + lineOffset,
					&Theme::green
				);

				// Text
				std::wstringstream textBuffer;
				textBuffer << (int32_t) (_value * 100);
				const auto text = textBuffer.str();

				renderer->renderString(
					Point(
						bounds.getX() + bounds.getWidth() / 2 - Theme::fontNormal.getWidth(text) / 2 + 1,
						bounds.getY() + frameHeight + textOffset
					),
					&Theme::fontNormal,
					&Theme::green,
					text
				);
			}

			float getValue() const {
				return _value;
			}

			void setValue(float value) {
				_value = value;

				invalidate();
			}

		private:
			float _value = 0;
	};
}
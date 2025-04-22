#pragma once

#include <format>
#include <YOBA/UI.h>
#include "../../../theme.h"

namespace pizda {
	using namespace YOBA;

	class ThrottleIndicator : public Element {
		public:
			void onRender(Renderer* renderer, const Bounds& bounds) override {
				const uint8_t lineOffset = 2;
				const uint8_t textOffset = 3;
				const uint16_t frameHeight = bounds.getHeight() - Theme::fontSmall.getHeight() - textOffset;

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
				const auto valueHeight = (uint16_t) (frameHeight * _value / 0xFFFF);
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
				const auto text = std::to_wstring((int32_t) (_value * 100 / 0xFFFF));

				renderer->renderString(
					Point(
						bounds.getX() + bounds.getWidth() / 2 - Theme::fontSmall.getWidth(text) / 2 + 1,
						bounds.getY() + frameHeight + textOffset
					),
					&Theme::fontSmall,
					&Theme::green,
					text
				);
			}

			uint16_t getValue() const {
				return _value;
			}

			void setValue(uint16_t value) {
				_value = value;

				invalidate();
			}

		private:
			uint16_t _value = 0;
	};
}
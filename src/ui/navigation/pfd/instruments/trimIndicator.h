#pragma once

#include "ui/element.h"
#include "ui/theme.h"

using namespace yoba;

namespace pizdanc {
	class TrimIndicator : public Element {
		public:
			void render(ScreenBuffer* screenBuffer) override {
				const auto& bounds = getBounds();

				const uint8_t triangleSize = 3;
				const uint8_t triangleOffset = 2;
				const uint8_t textOffset = 3;

				uint8_t textLength = 6;
				wchar_t text[textLength];
				uint16_t textWidth;
				uint16_t lineLength;

				// Vertical line
				screenBuffer->renderVerticalLine(
					Point(
						bounds.getX(),
						bounds.getY()
					),
					bounds.getHeight(),
					&Theme::fg1
				);

				// Horizontal lines
				auto drawHorizontalLine = [&](int32_t y, const wchar_t* title) {
					swprintf(text, textLength, title);
					textWidth = Theme::font.getWidth(text);

					lineLength = bounds.getWidth() - textOffset - textWidth - 1;

					// Line
					screenBuffer->renderHorizontalLine(
						Point(
							bounds.getX() + 1,
							y
						),
						lineLength,
						&Theme::fg1
					);
					
					// Text
					screenBuffer->renderText(
						Point(
							bounds.getX() + 1 + lineLength + textOffset,
							y - Theme::font.getHeight() / 2
						),
						&Theme::font,
						&Theme::fg1,
						text
					);
				};

				drawHorizontalLine(bounds.getY(), L"ND");
				drawHorizontalLine(bounds.getY2(), L"NU");

				// Suggested value
				screenBuffer->renderFilledRectangle(
					Bounds(
						bounds.getX(),
						bounds.getY() + (int32_t) (_suggestedMinimum * (float) bounds.getHeight()),
						3,
						(uint16_t) ((_suggestedMaximum - _suggestedMinimum) * (float) bounds.getHeight())
					),
					&Theme::green
				);

				// Triangle
				const auto trianglePosition = Point(
					bounds.getX() + triangleOffset + 1,
					bounds.getY() + (int32_t) (_value * (float) bounds.getHeight())
				);

				screenBuffer->renderFilledTriangle(
					Point(
						trianglePosition.getX() + triangleSize,
						trianglePosition.getY() - triangleSize
					),
					trianglePosition,
					Point(
						trianglePosition.getX() + triangleSize,
						trianglePosition.getY() + triangleSize
					),
					&Theme::yellow
				);

				// Text
				swprintf(text, textLength, L"%.1f", _value);

				screenBuffer->renderText(
					Point(
						trianglePosition.getX() + triangleSize + textOffset + 1,
						trianglePosition.getY()
					),
					&Theme::font,
					&Theme::yellow,
					text
				);
			}

			float getValue() const {
				return _value;
			}

			void setValue(float value) {
				_value = value;
			}

			float getSuggestedMinimum() const {
				return _suggestedMinimum;
			}

			void setSuggestedMinimum(float suggestedMinimum) {
				_suggestedMinimum = suggestedMinimum;
			}

			float getSuggestedMaximum() const {
				return _suggestedMaximum;
			}

			void setSuggestedMaximum(float suggestedMaximum) {
				_suggestedMaximum = suggestedMaximum;
			}

		private:
			float _suggestedMinimum = 0;
			float _suggestedMaximum = 0;
			float _value = 0;
	};
}
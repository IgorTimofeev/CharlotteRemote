#pragma once

#include "src/ui.h"
#include "../../../../theme.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class TrimIndicator : public Element {
		public:
			void onRender(Renderer* renderer, const Bounds& bounds) override {
				const uint8_t triangleSize = 2;
				const uint8_t suggestedValueSize = 2;

				if (_orientation == Orientation::vertical) {
					// Vertical line
					renderer->renderVerticalLine(
						bounds.getPosition(),
						bounds.getHeight(),
						&Theme::fg1
					);

					// Suggested value
					renderer->renderFilledRectangle(
						Bounds(
							bounds.getX(),
							bounds.getY() + _suggestedMinimum * bounds.getHeight() / 0xFFFF,
							suggestedValueSize,
							(_suggestedMaximum - _suggestedMinimum) * bounds.getHeight() / 0xFFFF
						),
						&Theme::green
					);

					// Triangle
					const auto trianglePosition = Point(
						bounds.getX2() - triangleSize,
						bounds.getY() + (int32_t) (_value * bounds.getHeight() / 0xFFFF)
					);

					renderer->renderFilledTriangle(
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
				}
				else {
					// Horizontal line
					renderer->renderHorizontalLine(
						bounds.getBottomLeft(),
						bounds.getWidth(),
						&Theme::fg1
					);

					// Suggested value
					renderer->renderFilledRectangle(
						Bounds(
							bounds.getX() + _suggestedMinimum * bounds.getWidth() / 0xFFFF,
							bounds.getY2() - suggestedValueSize + 1,
							(_suggestedMaximum - _suggestedMinimum) * bounds.getWidth() / 0xFFFF,
							suggestedValueSize
						),
						&Theme::green
					);

					// Triangle
					const auto trianglePosition = Point(
						bounds.getX() + _value * bounds.getWidth() / 0xFFFF,
						bounds.getY() + triangleSize
					);

					renderer->renderFilledTriangle(
						Point(
							trianglePosition.getX() - triangleSize,
							trianglePosition.getY() - triangleSize
						),
						trianglePosition,
						Point(
							trianglePosition.getX() + triangleSize,
							trianglePosition.getY() - triangleSize
						),
						&Theme::yellow
					);
				}
			}

			uint16_t getValue() const {
				return _value;
			}

			void setValue(uint16_t value) {
				_value = value;
			}

			uint16_t getSuggestedMinimum() const {
				return _suggestedMinimum;
			}

			void setSuggestedMinimum(uint16_t suggestedMinimum) {
				_suggestedMinimum = suggestedMinimum;
			}

			uint16_t getSuggestedMaximum() const {
				return _suggestedMaximum;
			}

			void setSuggestedMaximum(uint16_t suggestedMaximum) {
				_suggestedMaximum = suggestedMaximum;
			}

			Orientation getOrientation() const {
				return _orientation;
			}

			void setOrientation(Orientation orientation) {
				_orientation = orientation;
			}

		private:
			uint16_t _suggestedMinimum = 0;
			uint16_t _suggestedMaximum = 0;
			uint16_t _value = 0;
			Orientation _orientation = Orientation::vertical;
	};
}
#pragma once

#include "ui/element.h"
#include "ui/theme.h"

namespace pizdanc {
	using namespace yoba;

	class TrimIndicator : public Element {
		public:
			void render(Renderer* renderer) override {
				const auto& bounds = getBounds();

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
							bounds.getY() + (int32_t) (_suggestedMinimum * (float) bounds.getHeight()),
							suggestedValueSize,
							(uint16_t) ((_suggestedMaximum - _suggestedMinimum) * (float) bounds.getHeight())
						),
						&Theme::green
					);

					// Triangle
					const auto trianglePosition = Point(
						bounds.getX2() - triangleSize,
						bounds.getY() + (int32_t) (_value * (float) bounds.getHeight())
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
							bounds.getX() + (int32_t) (_suggestedMinimum * (float) bounds.getWidth()),
							bounds.getY2() - suggestedValueSize + 1,
							(uint16_t) ((_suggestedMaximum - _suggestedMinimum) * (float) bounds.getWidth()),
							suggestedValueSize
						),
						&Theme::green
					);

					// Triangle
					const auto trianglePosition = Point(
						bounds.getX() + (int32_t) (_value * (float) bounds.getWidth()),
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

			Orientation getOrientation() const {
				return _orientation;
			}

			void setOrientation(Orientation orientation) {
				_orientation = orientation;
			}

		private:
			float _suggestedMinimum = 0;
			float _suggestedMaximum = 0;
			float _value = 0;
			Orientation _orientation = Orientation::vertical;
	};
}
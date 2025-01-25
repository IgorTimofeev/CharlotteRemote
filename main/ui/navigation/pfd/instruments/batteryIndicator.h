#pragma once

#include <sstream>
#include "../../../components/yoba/src/ui.h"
#include "../../../theme.h"

namespace pizdanc {
	using namespace yoba;

	class BatteryIndicator : public Element {
		public:
			void render(Renderer* renderer) override {
				const auto& bounds = getBounds();
				const int32_t yCenter = bounds.getYCenter();

				const auto tipSize = Size(4, 6);
				const auto tipPosition = Point(bounds.getX2() - tipSize.getWidth(), yCenter - tipSize.getHeight() / 2);
				const uint16_t borderWidth = bounds.getWidth() - tipSize.getWidth();

				// Border
				renderer->renderRectangle(Bounds(bounds.getX(), bounds.getY(), borderWidth, bounds.getHeight()), 2, &Theme::bg4);

				// Tip
				renderer->renderRectangle(Bounds(tipPosition, tipSize), 2, &Theme::bg4);

				// Fill
				const Color* color;

				if (_value < 0.3) {
					color = &Theme::bad2;
				}
				else if (_value < 0.6) {
					color = &Theme::yellow;
				}
				else {
					color = &Theme::good2;
				}

				renderer->renderFilledRectangle(
					Bounds(
						bounds.getX(),
						bounds.getY(),
						(uint16_t) ((float) borderWidth * _value),
						bounds.getHeight()
					),
					2,
					color
				);

				// Text
				static std::wstringstream stream;
				stream.str(std::wstring());
				stream << std::round(_value * 100);
				const auto text = stream.str();

				renderer->renderString(
					Point(
						bounds.getX() + borderWidth / 2 - Theme::fontSmall.getWidth(text) / 2,
						bounds.getYCenter() - Theme::fontSmall.getHeight() / 2
					),
					&Theme::fontSmall,
					&Theme::fg1,
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
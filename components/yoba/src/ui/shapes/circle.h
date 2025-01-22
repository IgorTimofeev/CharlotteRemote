#pragma once

#include "main/color.h"
#include "rendering/renderers/renderer.h"
#include "shape.h"

namespace yoba::ui {
	class Circle : public Shape {
		public:
			Circle() = default;

			explicit Circle(const Color* color) {
				setFillColor(color);
			}

			void onRender(Renderer* renderer) override {
				auto color = getFillColor();

				if (!color)
					return;

				const auto& bounds = getBounds();

				renderer->renderFilledCircle(
					Point(bounds.getX() + bounds.getXCenter(), bounds.getY() + bounds.getYCenter() / 2),
					std::min(bounds.getWidth(), bounds.getHeight()) / 2,
					color
				);
			}
	};
}
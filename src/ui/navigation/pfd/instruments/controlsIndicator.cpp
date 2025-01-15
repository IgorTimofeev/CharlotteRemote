#include "controlsIndicator.h"
#include "../../../rc_application.h"

namespace pizdanc {
	ControlsIndicator::ControlsIndicator() {
		setImage(&_PFDCSImage);
	}

	void ControlsIndicator::render(Renderer* renderer) {
		ImageView::render(renderer);

		const auto& bounds = getBounds();
		auto& app = RCApplication::getInstance();

		// If surface rotation >= 5 deg
		const auto badValue = radians(5) / radians(maxAngle);

		// Lines
		auto ebanina = [&](const Point& point, uint16_t length, float angle) {
			const auto color = abs(angle) >= badValue ? &Theme::red : &Theme::green;

			// A little bit faster
			if (angle == 0) {
				renderer->renderHorizontalLine(point, length, color);
			}
			// BUT...
			else {
				renderer->renderLine(
					point,
					point + (Point) Vector2F(length, 0).rotate(angle),
					color
				);
			}
		};

		// Spoilers
		ebanina(
			Point(
				bounds.getX() + 17,
				bounds.getY() + 1
			),
			7,
			-app.getSpoilersInterpolator().getValue() * (float) radians(maxAngle)
		);

		// Flaps
		ebanina(
			Point(
				bounds.getX() + 31,
				bounds.getY() + 3
			),
			8,
			app.getFlapsInterpolator().getValue() * (float) radians(maxAngle)
		);
	}
}
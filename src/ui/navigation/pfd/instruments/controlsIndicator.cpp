#include "controlsIndicator.h"
#include "../../../rc_application.h"

namespace pizdanc {
	ControlsIndicator::ControlsIndicator() {
		setImage(&_PFDCSImage);
	}

	void ControlsIndicator::render(ScreenBuffer* screenBuffer) {
		ImageView::render(screenBuffer);

		const auto& bounds = getBounds();
		auto& app = RCApplication::getInstance();

		// Lines
		auto ebanina = [&](const Point& point, uint16_t length, float angle) {
			// A little bit faster
			if (angle == 0) {
				screenBuffer->renderHorizontalLine(point, length, &Theme::green);
			}
			// BUT...
			else {
				screenBuffer->renderLine(
					point,
					point + (Point) Vector2F(length, 0).rotate(angle),
					&Theme::green
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
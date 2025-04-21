#include "flapsAndSpoilersIndicator.h"
#include "../../../../rc.h"

namespace pizda {
	FlapsAndSpoilersIndicator::FlapsAndSpoilersIndicator() {
		setImage(&_image);
	}

	void FlapsAndSpoilersIndicator::onRender(Renderer* renderer, const Bounds& bounds) {
		ImageView::onRender(renderer, bounds);

		auto& rc = RC::getInstance();

		// If surface rotation >= 5 deg
		const auto badValue = toRadians(5) / toRadians(maxAngle);

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
			-rc.getLeverLeft().getMappedFloatValue() * (float) toRadians(maxAngle)
		);

		// Flaps
		ebanina(
			Point(
				bounds.getX() + 31,
				bounds.getY() + 3
			),
			8,
			rc.getLeverRight().getMappedFloatValue() * (float) toRadians(maxAngle)
		);
	}
}
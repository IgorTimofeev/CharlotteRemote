#include "flapsAndSpoilersIndicator.h"
#include "../../../../rc.h"

namespace pizda {
	FlapsAndSpoilersIndicator::FlapsAndSpoilersIndicator() {
		setImage(&resources::Images::MFDFlapsAndSpoilers);
	}

	void FlapsAndSpoilersIndicator::onRender(Renderer* renderer) {
		ImageView::onRender(renderer);

		const auto& bounds = getBounds();
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
					point + static_cast<Point>(Vector2F(length, 0).rotate(angle)),
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
			-rc.getLeverLeft().getMappedFloatValue() * toRadians(maxAngle)
		);

		// Flaps
		ebanina(
			Point(
				bounds.getX() + 31,
				bounds.getY() + 3
			),
			8,
			rc.getLeverRight().getMappedFloatValue() * toRadians(maxAngle)
		);
	}
}
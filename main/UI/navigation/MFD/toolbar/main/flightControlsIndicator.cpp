#include "flightControlsIndicator.h"

#include "UI/theme.h"
#include "rc.h"

namespace pizda {
	FlightControlsIndicator::FlightControlsIndicator() {
		setSize(Size(23, 23));
	}

	void FlightControlsIndicator::onRender(Renderer* renderer, const Bounds& bounds) {
		auto& rc = RC::getInstance();
		
		auto center = bounds.getCenter();
		auto radius = bounds.getWidth() / 2 - 2;
		
		renderer->renderCircle(center, radius, &Theme::fg7);
		
		// Blackout
		renderer->renderFilledRectangle(Bounds(bounds.getX(), center.getY() - 1, bounds.getWidth(), 3), &Theme::bg1);
		renderer->renderFilledRectangle(Bounds(center.getX() - 1, bounds.getY(), 3, bounds.getHeight()), &Theme::bg1);
		
		// Lines
		const auto lineLength = bounds.getWidth() / 2 - 1;
		renderer->renderHorizontalLine(Point(bounds.getX(), center.getY()), lineLength, &Theme::fg4);
		renderer->renderHorizontalLine(Point(center.getX() + 2, center.getY()), lineLength, &Theme::fg4);
		
		renderer->renderVerticalLine(Point(center.getX(), bounds.getY()), lineLength, &Theme::fg4);
		renderer->renderVerticalLine(Point(center.getX(), center.getY() + 2), lineLength, &Theme::fg4);
		
		// Joys
		const auto joyH = bounds.getX() + rc.getJoystickHorizontal().getMappedUint8Value() * bounds.getWidth() / 0xFF;
		const auto joyV = bounds.getY() + (0xFF - rc.getJoystickVertical().getMappedUint8Value()) * bounds.getWidth() / 0xFF;
		
		renderer->renderPixel(Point(joyH - 1, joyV), &Theme::yellow);
		renderer->renderPixel(Point(joyH + 1, joyV), &Theme::yellow);
		renderer->renderPixel(Point(joyH, joyV - 1), &Theme::yellow);
		renderer->renderPixel(Point(joyH, joyV + 1), &Theme::yellow);
		
		// Ring
		const auto ring = (rc.getRing().getMappedUint8Value() - 0xFF / 2) * bounds.getWidth() / 0xFF;
		renderer->renderHorizontalLine(
			Point(
				ring >= 0 ? center.getX() : center.getX() + ring,
				bounds.getY2()
			),
			std::abs(ring),
			&Theme::yellow
		);
		
		// Levers
		constexpr static uint8_t maxLeverHeight = 8;

		const auto levL = rc.getLeverLeft().getMappedUint8Value() * maxLeverHeight / 0xFF;
		const auto levR = rc.getLeverRight().getMappedUint8Value() * maxLeverHeight / 0xFF;

		renderer->renderVerticalLine(Point(bounds.getX(), center.getY()), levL, &Theme::bad3);
		renderer->renderVerticalLine(Point(bounds.getX2(), center.getY()), levR, &Theme::bad3);
	}
}
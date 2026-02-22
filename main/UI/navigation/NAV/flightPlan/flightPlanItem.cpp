#include "flightPlanItem.h"

#include "rc.h"

#include "flightPlanItemDialog.h"
#include "utilities/rendering.h"

namespace pizda {
	FlightPlanItem::FlightPlanItem(const uint16_t legIndex) : _legIndex(legIndex) {
		setWaypointIndex(RC::getInstance().getNavigationData().flightPlan.legs[legIndex].waypointIndex);
	}

	void FlightPlanItem::onClick() {
		(new FlightPlanItemDialog(_legIndex))->show();
	}

	void FlightPlanItem::onRender(Renderer& renderer, const Bounds& bounds) {
		if (getWaypointIndex() < 0)
			return;

		auto& rc = RC::getInstance();
		const auto& waypointData = rc.getNavigationData().waypoints[getWaypointIndex()];
		const auto isActiveLeg = rc.getNavigationData().flightPlan.activeLegIndex == _legIndex;

		// Background
		renderer.renderFilledRectangle(
			bounds,
			Theme::cornerRadius,
			isActive() ? Theme::bg4 : Theme::bg3
		);

		// Frame
		renderer.renderRectangle(
			bounds,
			Theme::cornerRadius,
			isActiveLeg ? Theme::magenta1 : (isActive() ? Theme::fg1 : Theme::bg4)
		);

		auto x = bounds.getX() + 15;
		const auto yCenter = bounds.getYCenter();
		const auto& color = RenderingUtils::getWaypointColor(waypointData);

		// Icon
		RenderingUtils::renderWaypointIcon(
			renderer,
			Point(x, yCenter),
			color,
			waypointData
		);

		x += 15;

		// Name
		renderer.renderText(
			Point(
				x,
				yCenter - Theme::fontNormal.getHeight() / 2
			),
			Theme::fontNormal,
			Theme::fg1,
			waypointData.name
		);

		// Distance
		const auto coordsText = std::format(
			L"{} nm",
			YOBA::round(getDistanceNm(), 1)
		);

		x = bounds.getX2() - 10 - Theme::fontNormal.getWidth(coordsText);

		renderer.renderText(
			Point(
				x,
				yCenter - Theme::fontNormal.getHeight() / 2
			),
			Theme::fontNormal,
			Theme::fg4,
			coordsText
		);

		// Active leg
		if (isActiveLeg && _legIndex > 0) {
			constexpr static uint8_t lineWidth = 5;
			constexpr static uint8_t arrowWidth = 4;
			constexpr static uint8_t arrowHeightDiv2 = 2;

			const auto lineHeight = bounds.getHeight() + 5;
			const auto yPrev = yCenter - lineHeight;

			x = bounds.getX() - lineWidth - arrowWidth;

			renderer.renderHorizontalLine(Point(x, yPrev), lineWidth, Theme::magenta1);
			renderer.renderVerticalLine(Point(x, yPrev + 1), lineHeight - 1, Theme::magenta1);
			renderer.renderHorizontalLine(Point(x, yCenter), lineWidth, Theme::magenta1);

			x += lineWidth;

			renderer.renderFilledTriangle(
				Point(x, yCenter - arrowHeightDiv2),
				Point(x + arrowWidth, yCenter),
				Point(x, yCenter + arrowHeightDiv2),
				Theme::magenta1
			);
		}
	}
}

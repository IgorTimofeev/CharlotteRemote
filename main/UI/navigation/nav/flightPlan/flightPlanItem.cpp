#include "flightPlanItem.h"

#include "rc.h"

#include "flightPlanItemDialog.h"
#include "utils/rendering.h"

namespace pizda {
	FlightPlanItem::FlightPlanItem(const uint16_t legIndex): _legIndex(legIndex) {
		setWaypointIndex(RC::getInstance().getNavigationData().flightPlan.legs[legIndex].waypointIndex);
	}

	void FlightPlanItem::onClick() {
		(new FlightPlanItemDialog(_legIndex))->show();
	}

	void FlightPlanItem::onRender(Renderer* renderer, const Bounds& bounds) {
		if (getWaypointIndex() < 0)
			return;

		auto& rc = RC::getInstance();
		const auto& waypointData = rc.getNavigationData().waypoints[getWaypointIndex()];

		renderer->renderFilledRectangle(
			bounds,
			Theme::cornerRadius,
			isActive() ? &Theme::bg4 : &Theme::bg3
		);

		renderer->renderRectangle(
			bounds,
			Theme::cornerRadius,
			isActive() ? &Theme::fg1 : &Theme::bg4
		);

		auto x = bounds.getX() + 15;
		const auto y = bounds.getYCenter();
		const Color* color;

		switch (waypointData.type) {
			case NavigationWaypointType::enroute:
				color = &Theme::magenta;

				break;

			default:
				color = &Theme::ocean;

				break;
		}

		RenderingUtils::renderWaypoint(
			renderer,
			Point(x, y),
			color,
			waypointData
		);

		x += 15;

		renderer->renderString(
			Point(
				x,
				y - Theme::fontNormal.getHeight() / 2
			),
			&Theme::fontNormal,
			&Theme::fg1,
			waypointData.name
		);

		// Distance
		const auto coordsText = std::format(
			L"{} nm",
			YOBA::round(getDistanceNm(), 1)
		);

		x = bounds.getX2() - 10 - Theme::fontNormal.getWidth(coordsText);

		renderer->renderString(
			Point(
				x,
				y - Theme::fontNormal.getHeight() / 2
			),
			&Theme::fontNormal,
			&Theme::fg4,
			coordsText
		);
	}
}

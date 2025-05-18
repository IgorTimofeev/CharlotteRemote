#include "waypointItem.h"

#include <rc.h>
#include "waypointItemDialog.h"

namespace pizda {
	WaypointItem::WaypointItem(uint16_t waypointIndex): waypointIndex(waypointIndex) {
		setHeight(32);

		auto& rc = RC::getInstance();
		const auto& waypointData = rc.getNavigationData().waypoints[waypointIndex];

		_distanceNm = YOBA::round(
			Units::convertDistance(
				(waypointData.cartesianCoordinates - rc.getAircraftData().geographicCoordinates.toCartesian()).getLength(),
				DistanceUnit::meter,
				DistanceUnit::nauticalMile
			),
			2
		);
	}

	void WaypointItem::onClick() {
		WaypointItemDialog::showCyka(waypointIndex);
	}

	void WaypointItem::onRender(Renderer* renderer) {
		const auto& bounds = getBounds();

		auto& rc = RC::getInstance();
		const auto& waypointData = rc.getNavigationData().waypoints[waypointIndex];

		constexpr static uint8_t cornerRadius = 3;
		constexpr static uint8_t nameFontScale = 1;

		renderer->renderFilledRectangle(
			bounds,
			cornerRadius,
			isActive() ? &Theme::bg4 : &Theme::bg3
		);

		renderer->renderRectangle(
			bounds,
			cornerRadius,
			isActive() ? &Theme::fg1 : &Theme::bg4
		);

		auto x = bounds.getX() + 15;
		const auto y = bounds.getYCenter();
		const Color* color;

		switch (waypointData.type) {
			case NavigationWaypointType::enroute:
				color = &Theme::purple;

				break;
			case NavigationWaypointType::terminal:
				color = &Theme::purple;

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
				y - Theme::fontNormal.getHeight() / 2 * nameFontScale
			),
			&Theme::fontNormal,
			&Theme::fg1,
			waypointData.name,
			nameFontScale
		);

		const auto coordsText = std::format(
			L"{} nm",
			YOBA::round(_distanceNm, 2)
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

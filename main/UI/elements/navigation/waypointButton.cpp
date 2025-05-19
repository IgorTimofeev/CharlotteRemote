#include "waypointButton.h"

#include <UI/theme.h>
#include <types/navigationData.h>
#include <utils/rendering.h>
#include <rc.h>

namespace pizda {
	WaypointButton::WaypointButton() {
		setHeight(height);
	}

	WaypointButton::WaypointButton(uint32_t waypointIndex): WaypointButton() {
		setWaypointIndex(waypointIndex);
	}

	int32_t WaypointButton::getWaypointIndex() const {
		return _waypointIndex;
	}

	void WaypointButton::setWaypointIndex(int32_t value) {
		_waypointIndex = value;

		if (_waypointIndex >= 0) {
			auto& rc = RC::getInstance();
			const auto& waypointData = rc.getNavigationData().waypoints[value];

			_distanceNm = YOBA::round(
				Units::convertDistance(
					(waypointData.cartesianCoordinates - rc.getAircraftData().geographicCoordinates.toCartesian()).getLength(),
					DistanceUnit::meter,
					DistanceUnit::nauticalMile
				),
				1
			);
		}

		invalidate();
	}

	void WaypointButton::onRender(Renderer* renderer) {
		if (_waypointIndex < 0)
			return;

		const auto& bounds = getBounds();

		const auto& waypointData = RC::getInstance().getNavigationData().waypoints[_waypointIndex];

		constexpr static uint8_t cornerRadius = 3;

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
				y - Theme::fontNormal.getHeight() / 2
			),
			&Theme::fontNormal,
			&Theme::fg1,
			waypointData.name
		);

		// Distance
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

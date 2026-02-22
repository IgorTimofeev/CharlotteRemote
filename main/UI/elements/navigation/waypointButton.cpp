#include "waypointButton.h"

#include "UI/theme.h"
#include "types/navigationData.h"
#include "utilities/rendering.h"
#include "rc.h"

namespace pizda {
	WaypointButton::WaypointButton() {
		setHeight(height);
	}

	WaypointButton::WaypointButton(const uint32_t waypointIndex): WaypointButton() {
		setWaypointIndex(waypointIndex);
	}

	int32_t WaypointButton::getWaypointIndex() const {
		return _waypointIndex;
	}

	void WaypointButton::setWaypointIndex(const int32_t value) {
		_waypointIndex = value;

		if (_waypointIndex >= 0) {
			auto& rc = RC::getInstance();
			const auto& waypointData = rc.getNavigationData().waypoints[value];

			_distanceNm = YOBA::round(
				Units::convertDistance(
					(waypointData.cartesianCoordinates - rc.getAircraftData().computed.coordinates.toCartesian()).getLength(),
					DistanceUnit::meter,
					DistanceUnit::nauticalMile
				),
				1
			);
		}

		invalidate();
	}

	float WaypointButton::getDistanceNm() const {
		return _distanceNm;
	}

	void WaypointButton::onRender(Renderer& renderer, const Bounds& bounds) {
		if (_waypointIndex < 0)
			return;

		const auto& waypointData = RC::getInstance().getNavigationData().waypoints[_waypointIndex];

		constexpr static uint8_t cornerRadius = 3;

		renderer.renderFilledRectangle(
			bounds,
			cornerRadius,
			isActive() ? &Theme::bg4 : &Theme::bg3
		);

		renderer.renderRectangle(
			bounds,
			cornerRadius,
			isActive() ? &Theme::fg1 : &Theme::bg4
		);

		auto x = bounds.getX() + 15;
		const auto y = bounds.getYCenter();

		const auto color = RenderingUtils::getWaypointColor(waypointData);

		RenderingUtils::renderWaypointIcon(
			renderer,
			Point(x, y),
			color,
			waypointData
		);

		x += 15;

		renderer.renderString(
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
			YOBA::round(_distanceNm, 1)
		);

		x = bounds.getX2() - 10 - Theme::fontNormal.getWidth(coordsText);

		renderer.renderString(
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

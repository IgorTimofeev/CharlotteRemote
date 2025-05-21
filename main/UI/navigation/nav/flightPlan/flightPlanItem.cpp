#include "flightPlanItem.h"

#include <rc.h>

#include "airportFlightPlanItemDialog.h"
#include "legFlightPlanItemDialog.h"

namespace pizda {
	FlightPlanItem::FlightPlanItem() {

	}

	void FlightPlanItem::onRender(Renderer* renderer) {
		if (getWaypointIndex() < 0)
			return;

		const auto& bounds = getBounds();

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
			getNameForRendering(waypointData)
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

	std::wstring FlightPlanItem::getNameForRendering(const NavigationWaypointData& waypointData) const {
		return waypointData.name;
	}

	AirportFlightPlanItem::AirportFlightPlanItem(bool destination) : _destination(destination) {

	}

	const std::optional<NavigationAirportAndRunwayIndicesData>& AirportFlightPlanItem::getAirportAndRunway() const {
		return _airportAndRunway;
	}

	void AirportFlightPlanItem::setAirportAndRunway(const std::optional<NavigationAirportAndRunwayIndicesData>& airportAndRunway) {
		_airportAndRunway = airportAndRunway;

		const auto& nd = RC::getInstance().getNavigationData();

		setWaypointIndex(
			_airportAndRunway.has_value()
			? nd.airports[_airportAndRunway.value().airportIndex].waypointIndex
			: -1
		);

		invalidate();
	}

	void AirportFlightPlanItem::onClick() {
		(new AirportFlightPlanItemDialog(_airportAndRunway.value(), _destination))->show();
	}

	std::wstring AirportFlightPlanItem::getNameForRendering(const NavigationWaypointData& waypointData) const {
		const auto& nd = RC::getInstance().getNavigationData();
		const auto& airport = nd.airports[_airportAndRunway.value().airportIndex];

		return std::format(L"{} / {}", waypointData.name, airport.runways[_airportAndRunway.value().runwayIndex].getFormattedName());
	}

	LegFlightPlanItem::LegFlightPlanItem(uint16_t legIndex): _legIndex(legIndex) {
		setWaypointIndex(RC::getInstance().getNavigationData().flightPlan.legs[legIndex].waypointIndex);
	}

	void LegFlightPlanItem::onClick() {
		(new LegFlightPlanItemDialog(_legIndex))->show();
	}
}

#include "flightPlanItem.h"

#include <rc.h>

#include "airportFlightPlanItemDialog.h"
#include "legFlightPlanItemDialog.h"

namespace pizda {
	FlightPlanItem::FlightPlanItem() {
		setHeight(32);
	}

	void FlightPlanItem::onRender(Renderer* renderer) {
		if (getWaypointIndex() < 0)
			return;

		const auto& bounds = getBounds();

		auto& rc = RC::getInstance();
		const auto& waypointData = rc.getNavigationData().waypoints[getWaypointIndex()];

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
	}

	AirportFlightPlanItem::AirportFlightPlanItem(bool destination) : _destination(destination) {

	}

	const std::optional<NavigationDataFlightPlanAirport>& AirportFlightPlanItem::getAirport() const {
		return _airport;
	}

	void AirportFlightPlanItem::setAirport(const std::optional<NavigationDataFlightPlanAirport>& airport) {
		_airport = airport;

		const auto& nd = RC::getInstance().getNavigationData();

		setWaypointIndex(
			_airport.has_value()
			? nd.airports[_airport.value().airportIndex].waypointIndex
			: -1
		);

		invalidate();
	}

	void AirportFlightPlanItem::onClick() {
		(new AirportFlightPlanItemDialog(_airport.value(), _destination))->show();
	}

	LegFlightPlanItem::LegFlightPlanItem(uint16_t legIndex): _legIndex(legIndex) {
		setWaypointIndex(RC::getInstance().getNavigationData().flightPlan.legs[legIndex].waypointIndex);
	}

	void LegFlightPlanItem::onClick() {
		(new LegFlightPlanItemDialog(getWaypointIndex()))->show();
	}
}

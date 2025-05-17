#include "navigationData.h"

#include <rc.h>

namespace pizda {
	NavigationWaypointData::NavigationWaypointData(
		NavigationWaypointType type,
		std::wstring_view name,
		const GeographicCoordinates& coordinates
	) :
		type(type),
		name(name),
		geographicCoordinates(coordinates),
		cartesianCoordinates(coordinates.toCartesian())
	{

	}

	NavigationRunwayData::NavigationRunwayData(
		const GeographicCoordinates& coordinates,
		uint16_t headingDeg,
		NavigationRunwayDataAlignment alignment,
		uint16_t lengthM,
		uint16_t widthM
	) :
		coordinates(coordinates),
		headingDeg(headingDeg),
		alignment(alignment),
		lengthM(lengthM),
		widthM(widthM)
	{
		// 0     1                           0     1
		// |--|--|                           |--|--|
		// |--|--|                          |--|--|
		// |--|--|                         |--|--|
		// |--C--|  > 10 deg heading >    |--C--|
		// |--|--|                       |--|--|
		// |--|--|                      |--|--|
		// |--|--|                     |--|--|
		// 3     2                     3     2
		const auto headingRad = toRadians(static_cast<float>(this->headingDeg));
		const auto& headingVectorNorm = Vector2F(std::sinf(headingRad), std::cosf(headingRad));
		const auto center01M = headingVectorNorm * (static_cast<float>(this->lengthM) / 2.f);
		const auto corner0OffsetM = headingVectorNorm.counterClockwisePerpendicular() * (static_cast<float>(this->widthM) / 2.f);
		const auto corner0Rad = (center01M + corner0OffsetM) * GeographicCoordinates::equatorialRadiansPerMeter;
		const auto corner1Rad = (center01M - corner0OffsetM) * GeographicCoordinates::equatorialRadiansPerMeter;

		// ESP_LOGI("Runway", "---------------------");
		// ESP_LOGI("Runway", "heading (deg): %d", _runway->headingDeg);
		// ESP_LOGI("Runway", "headingVectorNorm: %f, %f", headingVectorNorm.getX(), headingVectorNorm.getY());
		// ESP_LOGI("Runway", "center01Meters: %f, %f", center01M.getX(), center01M.getY());
		// ESP_LOGI("Runway", "corner0OffsetMeters: %f, %f", corner0OffsetM.getX(), corner0OffsetM.getY());
		// ESP_LOGI("Runway", "corner0Radians: %f, %f", corner0Rad.getX(), corner0Rad.getY());
		// ESP_LOGI("Runway", "corner1Radians: %f, %f", corner1Rad.getX(), corner1Rad.getY());

		vertices[0] = this->coordinates.toCartesian();
		vertices[1] = cornerToVertex(corner0Rad);
		vertices[2] = cornerToVertex(corner1Rad);
		vertices[3] = cornerToVertex(-corner0Rad);
		vertices[4] = cornerToVertex(-corner1Rad);
	}

	Vector3F NavigationRunwayData::cornerToVertex(const Vector2F& corner) const {
		const auto lat = coordinates.getLatitude() + corner.getY();

		return GeographicCoordinates(
			lat,
			coordinates.getLongitude() + corner.getX() / std::cosf(lat),
			coordinates.getAltitude()
		).toCartesian();
	}

	NavigationDataFlightPlanRoute::NavigationDataFlightPlanRoute(const NavigationWaypointData* from, const NavigationWaypointData* to) :
		from(from),
		to(to)
	{
		cartesianCoordinates[0] = this->from->cartesianCoordinates;
		cartesianCoordinates[1] = this->to->cartesianCoordinates;
	}

	void NavigationData::addAirport(
		std::wstring_view name,
		const GeographicCoordinates& coordinates,
		std::initializer_list<NavigationRunwayData> runways
	) {
		waypoints.push_back(NavigationWaypointData(
			NavigationWaypointType::airport,
			name,
			coordinates
		));

		ESP_LOGI("nav data", "AP address: %p", &waypoints[waypoints.size() - 1]);

		airports.push_back(NavigationAirportData(
			waypoints.size() - 1,
			runways
		));
	}

	void NavigationData::addRNAVWaypoint(
		NavigationWaypointType type,
		std::wstring_view name,
		const GeographicCoordinates& coordinates
	) {
		waypoints.push_back(NavigationWaypointData(
			type,
			name,
			coordinates
		));

		RNAVWaypoints.push_back(NavigationRNAVWaypointData(
			waypoints.size() - 1
		));
	}

	void NavigationData::fillWithTemplateData() {
		// Kronshtadt
		addAirport(
			L"ULLY",
			GeographicCoordinates(toRadians(60.014568277272f), toRadians(29.702727704862f), 0),
			{
				NavigationRunwayData(
					// 95
					GeographicCoordinates(toRadians(60.014568277272f), toRadians(29.702727704862f), 0),
					95,
					NavigationRunwayDataAlignment::none,
					500,
					30
				)
			}
		);

		// RNAV waypoints
		addRNAVWaypoint(
			NavigationWaypointType::enroute,
			L"OMEGA",
			GeographicCoordinates(toRadians(59.983333f), toRadians(30.133333f), 0)
		);

		addRNAVWaypoint(
			NavigationWaypointType::enroute,
			L"ABREL",
			GeographicCoordinates(toRadians(59.913056f), toRadians(31.335f), 0)
		);

		addRNAVWaypoint(
			NavigationWaypointType::enroute,
			L"SAPKI",
			GeographicCoordinates(toRadians(59.604722f), toRadians(31.180833f), 0)
		);

		addRNAVWaypoint(
			NavigationWaypointType::terminal,
			L"LI754",
			GeographicCoordinates(toRadians(59.516944f), toRadians(31.0225f), 0)
		);

		addRNAVWaypoint(
			NavigationWaypointType::terminal,
			L"OBARI",
			GeographicCoordinates(toRadians(59.599722f), toRadians(30.679167f), 0)
		);

		addRNAVWaypoint(
			NavigationWaypointType::terminal,
			L"BIPRI",
			GeographicCoordinates(toRadians(59.747778f), toRadians(30.565556f), 0)
		);

		// Pulkovo
		addAirport(
			L"ULLI",
			GeographicCoordinates(toRadians(59.800278f), toRadians(30.2625f), 0),
			{
				// 10 L
				NavigationRunwayData(
					GeographicCoordinates(toRadians(59.805114621892f), toRadians(30.276415586255f), 0),
					106,
					NavigationRunwayDataAlignment::none,
					3780,
					60
				),
				// 10 R
				NavigationRunwayData(
					GeographicCoordinates(toRadians(59.794929404415f), toRadians(30.251926678005f), 0),
					106,
					NavigationRunwayDataAlignment::none,
					3780,
					60
				)
			}
		);

		// Flight plan
		flightPlan.departure = NavigationDataFlightPlanAirport(&airports[0], 0);
		flightPlan.arrival = NavigationDataFlightPlanAirport(&airports[1], 0);

		// Route
		for (uint8_t i = 0; i < waypoints.size() - 1; i++) {
			flightPlan.routes.push_back(NavigationDataFlightPlanRoute(&waypoints[i], &waypoints[i + 1]));
		}
	}
}

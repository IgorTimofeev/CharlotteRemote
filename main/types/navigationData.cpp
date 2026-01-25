#include "navigationData.h"

#include <esp_log.h>
#include "rc.h"

namespace pizda {
	NavigationWaypointData::NavigationWaypointData(
		const NavigationWaypointType type,
		const std::wstring_view name,
		const GeoCoordinates& coordinates
	) :
		type(type),
		name(name),
		geographicCoordinates(coordinates),
		cartesianCoordinates(coordinates.toCartesian())
	{

	}

	NavigationRunwayData::NavigationRunwayData(
		const uint16_t waypointIndex,
		const uint16_t headingDeg,
		const uint16_t lengthM,
		const uint16_t widthM
	) :
		NavigationWaypointDataIndexAware(waypointIndex),
		headingDeg(headingDeg),
		lengthM(lengthM),
		widthM(widthM)
	{
		const auto& waypoint = RC::getInstance().getNavigationData().waypoints[this->waypointIndex];

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
		const auto corner0Rad = (center01M + corner0OffsetM) * GeoCoordinates::equatorialRadiansPerMeter;
		const auto corner1Rad = (center01M - corner0OffsetM) * GeoCoordinates::equatorialRadiansPerMeter;

		// ESP_LOGI("Runway", "---------------------");
		// ESP_LOGI("Runway", "heading (deg): %d", _runway->headingDeg);
		// ESP_LOGI("Runway", "headingVectorNorm: %f, %f", headingVectorNorm.getX(), headingVectorNorm.getY());
		// ESP_LOGI("Runway", "center01Meters: %f, %f", center01M.getX(), center01M.getY());
		// ESP_LOGI("Runway", "corner0OffsetMeters: %f, %f", corner0OffsetM.getX(), corner0OffsetM.getY());
		// ESP_LOGI("Runway", "corner0Radians: %f, %f", corner0Rad.getX(), corner0Rad.getY());
		// ESP_LOGI("Runway", "corner1Radians: %f, %f", corner1Rad.getX(), corner1Rad.getY());

		spatialVertices[0] = cornerToVertex(waypoint.geographicCoordinates, corner0Rad);
		spatialVertices[1] = cornerToVertex(waypoint.geographicCoordinates, corner1Rad);
		spatialVertices[2] = cornerToVertex(waypoint.geographicCoordinates, -corner0Rad);
		spatialVertices[3] = cornerToVertex(waypoint.geographicCoordinates, -corner1Rad);
		spatialVertices[4] = waypoint.geographicCoordinates.toCartesian();
	}

	Vector3F NavigationRunwayData::cornerToVertex(const GeoCoordinates& geographicCoordinates, const Vector2F& cornerRad) const {
		const auto lat = geographicCoordinates.getLatitude() + cornerRad.getY();

		return GeoCoordinates(
			lat,
			geographicCoordinates.getLongitude() + cornerRad.getX() / std::cosf(lat),
			geographicCoordinates.getAltitude()
		).toCartesian();
	}

	NavigationDataFlightPlanLeg::NavigationDataFlightPlanLeg(const uint16_t waypointIndex):
		NavigationWaypointDataIndexAware(waypointIndex)
	{

	}

	void NavigationData::addRunway(
		const std::wstring_view name,
		const GeoCoordinates& coordinates,
		const uint16_t headingDeg,
		const uint16_t lengthM,
		const uint16_t widthM
	) {
		waypoints.push_back(NavigationWaypointData(
			NavigationWaypointType::runway,
			name,
			coordinates
		));

		runways.push_back(NavigationRunwayData(
			waypoints.size() - 1,
			headingDeg,
			lengthM,
			widthM
		));
	}

	void NavigationData::addEnrouteWaypoint(
		const NavigationWaypointType type,
		const std::wstring_view name,
		const GeoCoordinates& coordinates
	) {
		waypoints.push_back(NavigationWaypointData(
			type,
			name,
			coordinates
		));

		enrouteWaypoints.push_back(NavigationEnrouteWaypointData(
			waypoints.size() - 1
		));
	}

	void NavigationData::clear() {
		waypoints.clear();
		enrouteWaypoints.clear();
		runways.clear();
		flightPlan.legs.clear();
	}

	void NavigationData::addTemplateData() {
		// Pulkovo 10 left
		addRunway(
			L"ULLI",
			GeoCoordinates(toRadians(59.805114621892f), toRadians(30.276415586255f), 0),
			// 10 L
			106,
			3780,
			60
		);

		// Kronshtadt
		addRunway(
			L"ULLY",
			GeoCoordinates(toRadians(60.014568277272f), toRadians(29.702727704862f), 0),
			95,
			500,
			30
		);

		// Gostilitsy
		addRunway(
			L"ULSG",
			GeoCoordinates(toRadians(59.725833f), toRadians(29.640278f), 0),
			154,
			711,
			20
		);

		// Enroute waypoints
		addEnrouteWaypoint(
			NavigationWaypointType::enroute,
			L"OMEGA",
			GeoCoordinates(toRadians(59.983333f), toRadians(30.133333f), 0)
		);

		addEnrouteWaypoint(
			NavigationWaypointType::enroute,
			L"ABREL",
			GeoCoordinates(toRadians(59.913056f), toRadians(31.335f), 0)
		);

		addEnrouteWaypoint(
			NavigationWaypointType::enroute,
			L"SAPKI",
			GeoCoordinates(toRadians(59.604722f), toRadians(31.180833f), 0)
		);

		addEnrouteWaypoint(
			NavigationWaypointType::enroute,
			L"LI754",
			GeoCoordinates(toRadians(59.516944f), toRadians(31.0225f), 0)
		);

		addEnrouteWaypoint(
			NavigationWaypointType::enroute,
			L"OBARI",
			GeoCoordinates(toRadians(59.599722f), toRadians(30.679167f), 0)
		);

		addEnrouteWaypoint(
			NavigationWaypointType::enroute,
			L"BIPRI",
			GeoCoordinates(toRadians(59.747778f), toRadians(30.565556f), 0)
		);

		// Flight plan
		flightPlan.legs.push_back(NavigationDataFlightPlanLeg(1));

		for (uint8_t i = 3; i < waypoints.size(); i++) {
			flightPlan.legs.push_back(NavigationDataFlightPlanLeg(i));
		}

		flightPlan.legs.push_back(NavigationDataFlightPlanLeg(0));
	}

	void NavigationData::removeWaypointAt(const uint16_t waypointIndex) {
		shiftWaypointIndicesAt(flightPlan.legs, waypointIndex);
		shiftWaypointIndicesAt(enrouteWaypoints, waypointIndex);
		shiftWaypointIndicesAt(runways, waypointIndex);

		waypoints.erase(waypoints.begin() + waypointIndex);
	}

	size_t NavigationData::getRunwayIndex(const uint16_t waypointIndex) const {
		return getWaypointIndexAwareVectorIndex(runways, waypointIndex);
	}

	size_t NavigationData::getEnrouteWaypointIndex(const uint16_t waypointIndex) const {
		return getWaypointIndexAwareVectorIndex(enrouteWaypoints, waypointIndex);
	}
}

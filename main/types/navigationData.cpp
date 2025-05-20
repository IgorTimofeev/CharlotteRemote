#include "navigationData.h"

#include <esp_log.h>
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
		const GeographicCoordinates& geographicCoordinates,
		uint16_t headingDeg,
		NavigationRunwayDataAlignment alignment,
		uint16_t lengthM,
		uint16_t widthM
	) :
		geographicCoordinates(geographicCoordinates),
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

		vertices[0] = this->geographicCoordinates.toCartesian();
		vertices[1] = cornerToVertex(corner0Rad);
		vertices[2] = cornerToVertex(corner1Rad);
		vertices[3] = cornerToVertex(-corner0Rad);
		vertices[4] = cornerToVertex(-corner1Rad);
	}

	Vector3F NavigationRunwayData::cornerToVertex(const Vector2F& corner) const {
		const auto lat = geographicCoordinates.getLatitude() + corner.getY();

		return GeographicCoordinates(
			lat,
			geographicCoordinates.getLongitude() + corner.getX() / std::cosf(lat),
			geographicCoordinates.getAltitude()
		).toCartesian();
	}

	NavigationDataFlightPlanLeg::NavigationDataFlightPlanLeg(uint16_t waypointIndex):
		NavigationWaypointDataIndexAware(waypointIndex)
	{

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

	void NavigationData::clear() {
		waypoints.clear();
		RNAVWaypoints.clear();
		airports.clear();

		flightPlan.origin = std::nullopt;
		flightPlan.legs.clear();
		flightPlan.destination = std::nullopt;
	}

	void NavigationData::addTemplateData() {
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
					NavigationRunwayDataAlignment::left,
					3780,
					60
				),
				// 10 R
				NavigationRunwayData(
					GeographicCoordinates(toRadians(59.794929404415f), toRadians(30.251926678005f), 0),
					106,
					NavigationRunwayDataAlignment::right,
					3780,
					60
				)
			}
		);

		// Flight plan
		flightPlan.origin = NavigationDataFlightPlanAirport(0, 0);

		// Legs
		for (uint8_t i = 1; i < waypoints.size() - 1; i++) {
			flightPlan.legs.push_back(NavigationDataFlightPlanLeg(i));
		}

		flightPlan.destination = NavigationDataFlightPlanAirport(1, 0);

		// Gostilitsy
		addAirport(
			L"ULSG",
			GeographicCoordinates(toRadians(59.725833f), toRadians(29.640278f), 0),
			{
				// 14
				NavigationRunwayData(
					GeographicCoordinates(toRadians(59.725833f), toRadians(29.640278f), 0),
					154,
					NavigationRunwayDataAlignment::none,
					711,
					20
				),
			}
		);

		// Domodedovo
		addAirport(
			L"UUDD",
			GeographicCoordinates(toRadians(55.409167f), toRadians(37.9075f), 0),
			{
				// 14 L
				NavigationRunwayData(
					GeographicCoordinates(toRadians(55.41046196776684f), toRadians(37.92864359192132f), 0),
					146,
					NavigationRunwayDataAlignment::left,
					3800,
					60
				),
				// 14 C
				NavigationRunwayData(
					GeographicCoordinates(toRadians(55.407978701847995f), toRadians(37.926114240209586f), 0),
					146,
					NavigationRunwayDataAlignment::center,
					2370,
					53
				),
				// 14 R
				NavigationRunwayData(
					GeographicCoordinates(toRadians(55.40872572644867f), toRadians(37.88697508989965f), 0),
					146,
					NavigationRunwayDataAlignment::right,
					3500,
					60
				),
			}
		);
	}

	void NavigationData::removeWaypointAt(uint16_t waypointIndex) {
		const auto& waypointData = waypoints[waypointIndex];

		if (waypointData.type == NavigationWaypointType::airport) {
			for (int32_t airportIndex = 0; airportIndex < airports.size(); airportIndex++) {
				auto& airport = airports[airportIndex];

				if (airport.waypointIndex == waypointIndex) {
					const auto handleFPLAirport = [airportIndex](std::optional<NavigationDataFlightPlanAirport>& airport) {
						if (airport.has_value()) {
							if (airport.value().airportIndex == airportIndex) {
								airport = std::nullopt;
							}
							else if (airport.value().airportIndex > airportIndex) {
								airport.value().airportIndex--;
							}
						}
					};

					handleFPLAirport(flightPlan.origin);
					handleFPLAirport(flightPlan.destination);

					airports.erase(airports.begin() + airportIndex);

					airportIndex--;
				}
				else if (airport.waypointIndex > waypointIndex) {
					airport.waypointIndex--;
				}
 			}
		}

		shiftWaypointIndicesAt(flightPlan.legs, waypointIndex);
		shiftWaypointIndicesAt(RNAVWaypoints, waypointIndex);

		waypoints.erase(waypoints.begin() + waypointIndex);
	}

	size_t NavigationData::getAirportIndex(uint16_t waypointIndex) const {
		return getWaypointIndexAwareVectorIndex(airports, waypointIndex);
	}

	size_t NavigationData::getRNAVWaypointIndex(uint16_t waypointIndex) const {
		return getWaypointIndexAwareVectorIndex(RNAVWaypoints, waypointIndex);
	}
}

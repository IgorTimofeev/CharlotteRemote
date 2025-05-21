#include "settingsNavigation.h"
#include <rc.h>

namespace pizda {
	const char* SettingsNavigation::getNVSNamespace() {
		return _namespace;
	}

	void SettingsNavigation::onRead(const NVSStream& stream) {
		auto& nd = RC::getInstance().getNavigationData();

		nd.clear();

		// Waypoints
		{
			const auto waypointsSize = stream.getUint16(_waypointsSize, 0);

			if (waypointsSize == 0) {
				nd.addTemplateData();
				return;
			}

			const auto waypoints = std::make_unique<SettingsNavigationWaypoint[]>(waypointsSize);
			stream.getObject(_waypointsList, waypoints.get(), waypointsSize);

			nd.waypoints.reserve(waypointsSize);

			for (uint16_t i = 0; i < waypointsSize; i++) {
				const auto& waypoint = waypoints[i];

				nd.waypoints.push_back(NavigationWaypointData(
					waypoint.type,
					waypoint.name,
					waypoint.geographicCoordinates
				));
			}
		}

		// RNAV
		{
			const auto RNAVWaypointsSize = stream.getUint16(_RNAVWaypointsSize, 0);

			if (RNAVWaypointsSize > 0) {
				const auto RNAVWaypoints = std::make_unique<SettingsNavigationRNAVWaypoint[]>(RNAVWaypointsSize);
				stream.getObject(_RNAVWaypointsList, RNAVWaypoints.get(), RNAVWaypointsSize);

				nd.RNAVWaypoints.reserve(RNAVWaypointsSize);

				for (uint16_t i = 0; i < RNAVWaypointsSize; i++) {
					const auto& RNAVWaypoint = RNAVWaypoints[i];

					nd.RNAVWaypoints.push_back(NavigationRNAVWaypointData(
						RNAVWaypoint.waypointIndex
					));
				}
			}
		}

		// Airports
		{
			const auto airportSize = stream.getUint16(_airportsSize, 0);

			if (airportSize > 0) {
				const auto airports = std::make_unique<SettingsNavigationAirport[]>(airportSize);
				stream.getObject(_airportsList, airports.get(), airportSize);

				nd.airports.reserve(airportSize);

				for (uint16_t i = 0; i < airportSize; i++) {
					const auto& airport = airports[i];

					nd.airports.push_back(NavigationAirportData(
						airport.waypointIndex,
						{}
					));
				}
			}
		}

		// Runways
		{
			const auto runwaysSize = stream.getUint32(_runwaysSize, 0);

			if (runwaysSize > 0) {
				const auto runways = std::make_unique<SettingsNavigationAirportRunway[]>(runwaysSize);
				stream.getObject(_runwaysList, runways.get(), runwaysSize);

				for (uint32_t i = 0; i < runwaysSize; i++) {
					const auto& runway = runways[i];

					nd.airports[runway.airportIndex].runways.push_back(NavigationRunwayData(
						runway.geographicCoordinates,
						runway.headingDeg,
						runway.alignment,
						runway.lengthM,
						runway.widthM
					));
				}
			}
		}

		// Flight plan
		{
			// Origin
			nd.flightPlan.origin =
				stream.getBool(_flightPlanOriginExists)
				? std::optional(NavigationDataFlightPlanAirport(stream.getUint16(_flightPlanOriginAirportIndex), stream.getUint8(_flightPlanOriginRunwayIndex)))
				: std::nullopt;

			// Legs
			{
				const auto legsSize = stream.getUint16(_flightPlanLegsSize);

				if (legsSize > 0) {
					nd.flightPlan.legs.reserve(legsSize);

					const auto legs = std::make_unique<SettingsNavigationFlightPlanLeg[]>(legsSize);
					stream.getObject(_flightPlanLegsList, legs.get(), legsSize);

					for (uint16_t i = 0; i < legsSize; i++) {
						const auto& leg = legs[i];

						nd.flightPlan.legs.push_back(NavigationDataFlightPlanLeg(
							leg.waypointIndex
						));
					}
				}
			}

			// Destination
			nd.flightPlan.destination =
				stream.getBool(_flightPlanDestinationExists)
				? std::optional(NavigationDataFlightPlanAirport(stream.getUint16(_flightPlanDestinationAirportIndex), stream.getUint8(_flightPlanDestinationRunwayIndex)))
				: std::nullopt;
		}
	}

	void SettingsNavigation::onWrite(const NVSStream& stream) {
		const auto& nd = RC::getInstance().getNavigationData();

		// Waypoints
		{
			stream.setUint16(_waypointsSize, nd.waypoints.size());

			const auto waypoints = std::make_unique<SettingsNavigationWaypoint[]>(nd.waypoints.size());

			for (uint16_t i = 0; i < nd.waypoints.size(); i++) {
				const auto& waypointData = nd.waypoints[i];

				waypoints[i] = SettingsNavigationWaypoint(
					waypointData.type,
					waypointData.name,
					waypointData.geographicCoordinates
				);
			}

			stream.setObject(_waypointsList, waypoints.get(), nd.waypoints.size());
		}

		// RNAV
		{
			stream.setUint16(_RNAVWaypointsSize, nd.RNAVWaypoints.size());

			const auto RNAVWaypoints = std::make_unique<SettingsNavigationRNAVWaypoint[]>(nd.RNAVWaypoints.size());

			for (uint16_t i = 0; i < nd.RNAVWaypoints.size(); i++) {
				const auto& RNAVWaypoint = nd.RNAVWaypoints[i];

				RNAVWaypoints[i] = SettingsNavigationRNAVWaypoint(
					RNAVWaypoint.waypointIndex
				);
			}

			stream.setObject(_RNAVWaypointsList, RNAVWaypoints.get(), nd.RNAVWaypoints.size());
		}

		// Airports
		{
			stream.setUint16(_airportsSize, nd.airports.size());

			const auto airports = std::make_unique<SettingsNavigationAirport[]>(nd.airports.size());

			size_t totalRunwaysSize = 0;

			for (size_t ai = 0; ai < nd.airports.size(); ai++) {
				const auto& airportData = nd.airports[ai];

				airports[ai] = SettingsNavigationAirport(
					airportData.waypointIndex
				);

				totalRunwaysSize += airportData.runways.size();
			}

			stream.setObject(_airportsList, airports.get(), nd.airports.size());

			// Runways
			{
				stream.setUint32(_runwaysSize, totalRunwaysSize);

				const auto runways = std::make_unique<SettingsNavigationAirportRunway[]>(totalRunwaysSize);

				size_t runwayIndex = 0;

				for (size_t ai = 0; ai < nd.airports.size(); ai++) {
					const auto& airportData = nd.airports[ai];

					for (size_t ri = 0; ri < airportData.runways.size(); ri++) {
						const auto& runwayData = airportData.runways[ri];

						runways[runwayIndex] = SettingsNavigationAirportRunway(
							ai,
							runwayData.geographicCoordinates,
							runwayData.headingDeg,
							runwayData.alignment,
							runwayData.lengthM,
							runwayData.widthM
						);

						runwayIndex++;
					}
				}

				stream.setObject(_runwaysList, runways.get(), totalRunwaysSize);
			}
		}

		// Flight plan
		{
			// Origin
			stream.setBool(_flightPlanOriginExists, nd.flightPlan.origin.has_value());
			stream.setUint16(_flightPlanOriginAirportIndex, nd.flightPlan.origin.has_value() ? nd.flightPlan.origin.value().airportIndex : 0);
			stream.setUint8(_flightPlanOriginRunwayIndex, nd.flightPlan.origin.has_value() ? nd.flightPlan.origin.value().runwayIndex : 0);

			// Legs
			stream.setUint16(_flightPlanLegsSize, nd.flightPlan.legs.size());

			const auto legs = std::make_unique<SettingsNavigationFlightPlanLeg[]>(nd.flightPlan.legs.size());

			for (uint16_t i = 0; i < nd.flightPlan.legs.size(); i++) {
				const auto& leg = nd.flightPlan.legs[i];

				legs[i] = SettingsNavigationFlightPlanLeg(
					leg.waypointIndex
				);
			}

			stream.setObject(_flightPlanLegsList, legs.get(), nd.flightPlan.legs.size());

			// Destination
			stream.setBool(_flightPlanDestinationExists, nd.flightPlan.destination.has_value());
			stream.setUint16(_flightPlanDestinationAirportIndex, nd.flightPlan.destination.has_value() ? nd.flightPlan.destination.value().airportIndex : 0);
			stream.setUint8(_flightPlanDestinationRunwayIndex, nd.flightPlan.destination.has_value() ? nd.flightPlan.destination.value().runwayIndex : 0);
		}
	}
}

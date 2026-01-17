#include "navigationSettings.h"

#include "rc.h"

namespace pizda {
	const char* NavigationSettings::getNamespace() {
		return _namespace;
	}

	void NavigationSettings::onRead(const NVSStream& stream) {
		auto& nd = RC::getInstance().getNavigationData();

		nd.clear();

		// Waypoints
		{
			const auto waypointsSize = stream.readUint16(_waypointsSize, 0);

			if (waypointsSize == 0) {
				nd.addTemplateData();
				return;
			}

			const auto waypoints = std::make_unique<NavigationSettingsWaypoint[]>(waypointsSize);
			stream.readObject(_waypointsList, waypoints.get(), waypointsSize);

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
			const auto RNAVWaypointsSize = stream.readUint16(_RNAVWaypointsSize, 0);

			if (RNAVWaypointsSize > 0) {
				const auto RNAVWaypoints = std::make_unique<NavigationSettingsRNAVWaypoint[]>(RNAVWaypointsSize);
				stream.readObject(_RNAVWaypointsList, RNAVWaypoints.get(), RNAVWaypointsSize);

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
			const auto airportSize = stream.readUint16(_airportsSize, 0);

			if (airportSize > 0) {
				const auto airports = std::make_unique<NavigationSettingsAirport[]>(airportSize);
				stream.readObject(_airportsList, airports.get(), airportSize);

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
			const auto runwaysSize = stream.readUint32(_runwaysSize, 0);

			if (runwaysSize > 0) {
				const auto runways = std::make_unique<NavigationSettingsAirportRunway[]>(runwaysSize);
				stream.readObject(_runwaysList, runways.get(), runwaysSize);

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
				stream.readBool(_flightPlanOriginExists)
				? std::optional(NavigationDataFlightPlanAirport(stream.readUint16(_flightPlanOriginAirportIndex), stream.readUint8(_flightPlanOriginRunwayIndex)))
				: std::nullopt;

			// Legs
			{
				const auto legsSize = stream.readUint16(_flightPlanLegsSize);

				if (legsSize > 0) {
					nd.flightPlan.legs.reserve(legsSize);

					const auto legs = std::make_unique<NavigationSettingsFlightPlanLeg[]>(legsSize);
					stream.readObject(_flightPlanLegsList, legs.get(), legsSize);

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
				stream.readBool(_flightPlanDestinationExists)
				? std::optional(NavigationDataFlightPlanAirport(stream.readUint16(_flightPlanDestinationAirportIndex), stream.readUint8(_flightPlanDestinationRunwayIndex)))
				: std::nullopt;
		}
	}

	void NavigationSettings::onWrite(const NVSStream& stream) {
		const auto& nd = RC::getInstance().getNavigationData();

		// Waypoints
		{
			stream.writeUint16(_waypointsSize, nd.waypoints.size());

			const auto waypoints = std::make_unique<NavigationSettingsWaypoint[]>(nd.waypoints.size());

			for (uint16_t i = 0; i < nd.waypoints.size(); i++) {
				const auto& waypointData = nd.waypoints[i];

				waypoints[i] = NavigationSettingsWaypoint(
					waypointData.type,
					waypointData.name,
					waypointData.geographicCoordinates
				);
			}

			stream.writeObject(_waypointsList, waypoints.get(), nd.waypoints.size());
		}

		// RNAV
		{
			stream.writeUint16(_RNAVWaypointsSize, nd.RNAVWaypoints.size());

			const auto RNAVWaypoints = std::make_unique<NavigationSettingsRNAVWaypoint[]>(nd.RNAVWaypoints.size());

			for (uint16_t i = 0; i < nd.RNAVWaypoints.size(); i++) {
				const auto& RNAVWaypoint = nd.RNAVWaypoints[i];

				RNAVWaypoints[i] = NavigationSettingsRNAVWaypoint(
					RNAVWaypoint.waypointIndex
				);
			}

			stream.writeObject(_RNAVWaypointsList, RNAVWaypoints.get(), nd.RNAVWaypoints.size());
		}

		// Airports
		{
			stream.writeUint16(_airportsSize, nd.airports.size());

			const auto airports = std::make_unique<NavigationSettingsAirport[]>(nd.airports.size());

			size_t totalRunwaysSize = 0;

			for (size_t ai = 0; ai < nd.airports.size(); ai++) {
				const auto& airportData = nd.airports[ai];

				airports[ai] = NavigationSettingsAirport(
					airportData.waypointIndex
				);

				totalRunwaysSize += airportData.runways.size();
			}

			stream.writeObject(_airportsList, airports.get(), nd.airports.size());

			// Runways
			{
				stream.writeUint32(_runwaysSize, totalRunwaysSize);

				const auto runways = std::make_unique<NavigationSettingsAirportRunway[]>(totalRunwaysSize);

				size_t runwayIndex = 0;

				for (size_t ai = 0; ai < nd.airports.size(); ai++) {
					const auto& airportData = nd.airports[ai];

					for (size_t ri = 0; ri < airportData.runways.size(); ri++) {
						const auto& runwayData = airportData.runways[ri];

						runways[runwayIndex] = NavigationSettingsAirportRunway(
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

				stream.writeObject(_runwaysList, runways.get(), totalRunwaysSize);
			}
		}

		// Flight plan
		{
			// Origin
			stream.writeBool(_flightPlanOriginExists, nd.flightPlan.origin.has_value());
			stream.writeUint16(_flightPlanOriginAirportIndex, nd.flightPlan.origin.has_value() ? nd.flightPlan.origin.value().airportIndex : 0);
			stream.writeUint8(_flightPlanOriginRunwayIndex, nd.flightPlan.origin.has_value() ? nd.flightPlan.origin.value().runwayIndex : 0);

			// Legs
			stream.writeUint16(_flightPlanLegsSize, nd.flightPlan.legs.size());

			const auto legs = std::make_unique<NavigationSettingsFlightPlanLeg[]>(nd.flightPlan.legs.size());

			for (uint16_t i = 0; i < nd.flightPlan.legs.size(); i++) {
				const auto& leg = nd.flightPlan.legs[i];

				legs[i] = NavigationSettingsFlightPlanLeg(
					leg.waypointIndex
				);
			}

			stream.writeObject(_flightPlanLegsList, legs.get(), nd.flightPlan.legs.size());

			// Destination
			stream.writeBool(_flightPlanDestinationExists, nd.flightPlan.destination.has_value());
			stream.writeUint16(_flightPlanDestinationAirportIndex, nd.flightPlan.destination.has_value() ? nd.flightPlan.destination.value().airportIndex : 0);
			stream.writeUint8(_flightPlanDestinationRunwayIndex, nd.flightPlan.destination.has_value() ? nd.flightPlan.destination.value().runwayIndex : 0);
		}
	}
}

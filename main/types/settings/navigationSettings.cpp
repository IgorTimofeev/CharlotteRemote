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
			const uint16_t waypointsLength = stream.readObjectLength<NavigationSettingsWaypoint>(_waypointsList);

			if (waypointsLength == 0) {
				nd.addTemplateData();
				return;
			}

			const auto waypoints = std::make_unique<NavigationSettingsWaypoint[]>(waypointsLength);
			stream.readObject(_waypointsList, waypoints.get(), waypointsLength);

			nd.waypoints.reserve(waypointsLength);

			for (uint16_t i = 0; i < waypointsLength; i++) {
				const auto& waypoint = waypoints[i];

				nd.waypoints.push_back(NavigationWaypointData(
					waypoint.type,
					waypoint.name,
					waypoint.geographicCoordinates
				));
			}
		}

		// Enroute
		{
			const uint16_t enrouteWaypointsLength = stream.readObjectLength<NavigationSettingsEnrouteWaypoint>(_enrouteWaypointsList);

			if (enrouteWaypointsLength > 0) {
				const auto enrouteWaypoints = std::make_unique<NavigationSettingsEnrouteWaypoint[]>(enrouteWaypointsLength);
				stream.readObject(_enrouteWaypointsList, enrouteWaypoints.get(), enrouteWaypointsLength);

				nd.enrouteWaypoints.reserve(enrouteWaypointsLength);

				for (uint16_t i = 0; i < enrouteWaypointsLength; i++) {
					const auto& enrouteWaypoint = enrouteWaypoints[i];

					nd.enrouteWaypoints.push_back(NavigationEnrouteWaypointData(
						enrouteWaypoint.waypointIndex
					));
				}
			}
		}

		// Runways
		{
			const uint16_t runwaysLength = stream.readObjectLength<NavigationSettingsRunway>(_runwaysList);

			if (runwaysLength > 0) {
				const auto runways = std::make_unique<NavigationSettingsRunway[]>(runwaysLength);
				stream.readObject(_runwaysList, runways.get(), runwaysLength);

				nd.runways.reserve(runwaysLength);

				for (uint16_t i = 0; i < runwaysLength; i++) {
					const auto& runway = runways[i];

					nd.runways.push_back(NavigationRunwayData(
						runway.waypointIndex,
						runway.headingDeg,
						runway.lengthM,
						runway.widthM
					));
				}
			}
		}

		// Flight plan
		{
			// Legs
			{
				const uint16_t legsLength = stream.readObjectLength<NavigationSettingsFlightPlanLeg>(_flightPlanLegsList);

				if (legsLength > 0) {
					nd.flightPlan.legs.reserve(legsLength);

					const auto legs = std::make_unique<NavigationSettingsFlightPlanLeg[]>(legsLength);
					stream.readObject(_flightPlanLegsList, legs.get(), legsLength);

					for (uint16_t i = 0; i < legsLength; i++) {
						const auto& leg = legs[i];

						nd.flightPlan.legs.push_back(NavigationDataFlightPlanLeg(
							leg.waypointIndex
						));
					}
				}
			}
		}
	}

	void NavigationSettings::onWrite(const NVSStream& stream) {
		const auto& nd = RC::getInstance().getNavigationData();

		// Waypoints
		{
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

		// Enroute
		{
			const auto enrouteWaypoints = std::make_unique<NavigationSettingsEnrouteWaypoint[]>(nd.enrouteWaypoints.size());

			for (uint16_t i = 0; i < nd.enrouteWaypoints.size(); i++) {
				const auto& enrouteWaypoint = nd.enrouteWaypoints[i];

				enrouteWaypoints[i] = NavigationSettingsEnrouteWaypoint(
					enrouteWaypoint.waypointIndex
				);
			}

			stream.writeObject(_enrouteWaypointsList, enrouteWaypoints.get(), nd.enrouteWaypoints.size());
		}

		// Runways
		{
			const auto runways = std::make_unique<NavigationSettingsRunway[]>(nd.runways.size());

			for (size_t i = 0; i < nd.runways.size(); i++) {
				const auto& runwayData = nd.runways[i];

				runways[i] = NavigationSettingsRunway(
					runwayData.waypointIndex,
					runwayData.headingDeg,
					runwayData.lengthM,
					runwayData.widthM
				);
			}

			stream.writeObject(_runwaysList, runways.get(), nd.runways.size());
		}

		// Flight plan
		{
			// Legs
			const auto legs = std::make_unique<NavigationSettingsFlightPlanLeg[]>(nd.flightPlan.legs.size());

			for (uint16_t i = 0; i < nd.flightPlan.legs.size(); i++) {
				const auto& leg = nd.flightPlan.legs[i];

				legs[i] = NavigationSettingsFlightPlanLeg(
					leg.waypointIndex
				);
			}

			stream.writeObject(_flightPlanLegsList, legs.get(), nd.flightPlan.legs.size());
		}
	}
}

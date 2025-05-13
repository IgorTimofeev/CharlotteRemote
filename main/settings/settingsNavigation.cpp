#include "settingsNavigation.h"
#include "../rc.h"

namespace pizda {
	void SettingsNavigation::onRead(const NVSStream& stream) {
		// Waypoints
		uint16_t size = stream.getUint16(_waypointsSize, 0);

		if (size > 0) {
			// Waypoints
			SettingsNavigationWaypoint waypoints[size] {};

			stream.getBlob(
				_waypointsList,
				reinterpret_cast<uint8_t*>(waypoints),
				sizeof(SettingsNavigationWaypoint) * size
			);

			// Runways
			size = stream.getUint16(_runwaysSize, 0);

			if (size > 0) {
				SettingsNavigationAirportRunway runways[size] {};

				stream.getBlob(
					_runwaysList,
					reinterpret_cast<uint8_t*>(runways),
					sizeof(SettingsNavigationAirportRunway) * size
				);
			}
		}
		else {
			RC::getInstance().getNavigationData().fillWithTemplateData();
		}
	}

	void SettingsNavigation::onWrite(const NVSStream& stream) {
		// // Waypoints
		// stream.setUint16(_waypointsSize, waypoints.size());
		//
		// stream.setBlob(
		// 	_waypointsList,
		// 	reinterpret_cast<uint8_t*>(waypoints.data()),
		// 	sizeof(SettingsNavigationWaypoint) * waypoints.size()
		// );
		//
		// // Runways
		// stream.setUint16(_runwaysSize, runways.size());
		//
		// stream.setBlob(
		// 	_runwaysList,
		// 	reinterpret_cast<uint8_t*>(runways.data()),
		// 	sizeof(SettingsNavigationAirportRunway) * runways.size()
		// );
	}
}

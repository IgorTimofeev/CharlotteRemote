#include "RNAVWaypointElement.h"


#include "../../../../../rc.h"
#include "../../../../theme.h"
#include "../ND.h"

namespace pizda {
	RNAVWaypointElement::RNAVWaypointElement(const SettingsNavigationRNAVWaypoint* waypoint) {
		_waypoint = &RC::getInstance().getSettings().navigation.waypoints[waypoint->waypointIndex];
		_vertex = waypoint->coordinates.toCartesian();
	}

	void RNAVWaypointElement::onRender(Renderer* renderer, const Scene& scene, const Vector3F* vertices) {
		ND::renderWaypointStar(
			renderer,
			_waypoint,
			Point(
				static_cast<int32_t>(vertices[0].getX()),
				static_cast<int32_t>(vertices[0].getY())
			),
			&Theme::ocean
		);
	}

	const Vector3F* RNAVWaypointElement::getVertices() {
		return &_vertex;
	}

	uint16_t RNAVWaypointElement::getVertexCount() {
		return 1;
	}
}

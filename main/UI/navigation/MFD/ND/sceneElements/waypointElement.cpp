#include "waypointElement.h"

#include <rc.h>
#include <UI/theme.h>
#include <UI/navigation/MFD/ND/NDScene.h>
#include <utils/rendering.h>

namespace pizda {
	WaypointElement::WaypointElement(uint16_t waypointIndex) : waypointIndex(waypointIndex) {

	}

	void WaypointElement::onRender(Renderer* renderer, const Scene& scene, const Vector3F* vertices) {
		const auto& waypointData = RC::getInstance().getNavigationData().waypoints[waypointIndex];

		const auto center = Point(
			static_cast<int32_t>(vertices[0].getX()),
			static_cast<int32_t>(vertices[0].getY())
		);

		const auto color = waypointData.type == NavigationWaypointType::airport ? &Theme::ocean : &Theme::fg1;

		RenderingUtils::renderWaypoint(
			renderer,
			center,
			color,
			waypointData
		);

		renderer->renderString(
			Point(center.getX() + 7, center.getY() - 7),
			&Theme::fontSmall,
			color,
			waypointData.name
		);
	}

	const Vector3F* WaypointElement::getVertices() {
		return &RC::getInstance().getNavigationData().waypoints[waypointIndex].cartesianCoordinates;
	}

	uint16_t WaypointElement::getVertexCount() {
		return 1;
	}
}

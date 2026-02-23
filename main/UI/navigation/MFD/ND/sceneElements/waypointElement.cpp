#include "waypointElement.h"

#include "rc.h"
#include "UI/theme.h"
#include "UI/navigation/MFD/ND/NDScene.h"
#include "utilities/rendering.h"

namespace pizda {
	WaypointElement::WaypointElement(const uint16_t waypointIndex) : waypointIndex(waypointIndex) {

	}

	void WaypointElement::onRender(Renderer& renderer, const Scene& scene, const Vector3F* projectedVertices) {
		if (projectedVertices[0].getZ() < scene.getNearPlaneDistance())
			return;

		const auto& waypointData = RC::getInstance().getNavigationData().waypoints[waypointIndex];

		const auto& waypointPosition = Point(
			static_cast<int32_t>(projectedVertices[0].getX()),
			static_cast<int32_t>(projectedVertices[0].getY())
		);

		RenderingUtils::renderWaypointIcon(
			renderer,
			waypointPosition,
			&Theme::magenta1,
			waypointData
		);

		RenderingUtils::renderWaypointName(
			renderer,
			waypointPosition,
			&Theme::fg1,
			waypointData
		);
	}

	const Vector3F* WaypointElement::getVertices() {
		return &RC::getInstance().getNavigationData().waypoints[waypointIndex].cartesianCoordinates;
	}

	uint16_t WaypointElement::getVertexCount() {
		return 1;
	}
}

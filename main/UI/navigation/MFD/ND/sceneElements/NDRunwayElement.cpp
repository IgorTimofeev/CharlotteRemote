#include "NDRunwayElement.h"

#include "UI/theme.h"
#include "utils/rendering.h"
#include "rc.h"

namespace pizda {
	NDRunwayElement::NDRunwayElement(const uint16_t runwayIndex) : RunwayElement(runwayIndex, &Theme::fg1) {

	}

	void NDRunwayElement::onRender(Renderer* renderer, const Scene& scene, const Vector3F* projectedVertices) {
		const auto& waypointVertex = projectedVertices[4];

		if (waypointVertex.getZ() < scene.getNearPlaneDistance())
			return;

		auto& rc = RC::getInstance();
		const auto& runway = rc.getNavigationData().runways[getRunwayIndex()];
		const auto& waypointData = rc.getNavigationData().waypoints[runway.waypointIndex];

		const auto& waypointPosition = Point(
			static_cast<int32_t>(waypointVertex.getX()),
			static_cast<int32_t>(waypointVertex.getY())
		);

		// Too far, rendering icon instead of mesh
		if (waypointVertex.getZ() > 16'000) {
			RenderingUtils::renderWaypointIcon(
				renderer,
				waypointPosition,
				&Theme::fg1,
				waypointData
			);
		}
		// Close enough, rendering runway mesh
		else {
			RunwayElement::onRender(renderer, scene, projectedVertices);
		}

		RenderingUtils::renderWaypointName(
			renderer,
			waypointPosition,
			&Theme::fg1,
			waypointData
		);
	}
}
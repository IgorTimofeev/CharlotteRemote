#include "NDRunwayElement.h"

#include "UI/theme.h"
#include "utilities/rendering.h"
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

		const auto waypointColor = RenderingUtils::getWaypointColor(waypointData);

		// Close enough, rendering runway mesh
		if (waypointVertex.getZ() <= 16'000)
			RunwayElement::onRender(renderer, scene, projectedVertices);

		RenderingUtils::renderWaypointIcon(
			renderer,
			waypointPosition,
			waypointColor,
			waypointData
		);

		RenderingUtils::renderWaypointName(
			renderer,
			waypointPosition,
			waypointColor,
			waypointData
		);
	}
}
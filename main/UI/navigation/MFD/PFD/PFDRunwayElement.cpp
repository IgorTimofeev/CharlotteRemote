#include "PFDRunwayElement.h"

#include "rc.h"
#include "UI/theme.h"

namespace pizda {
	PFDRunwayElement::PFDRunwayElement(const uint16_t runwayIndex): RunwayElement(runwayIndex, &Theme::bg1) {

	}

	void PFDRunwayElement::onRender(Renderer* renderer, const Scene& scene, const Vector3F* projectedVertices) {
		const auto& waypointVertex = projectedVertices[4];

		if (waypointVertex.getZ() < scene.getNearPlaneDistance())
			return;

		auto& rc = RC::getInstance();

		// Close enough, rendering runway mesh
		if (waypointVertex.getZ() <= 16'000) {
			RunwayElement::onRender(renderer, scene, projectedVertices);
		}

		// Waypoint labels
		if (!rc.getSettings().personalization.MFD.PFD.waypointLabels || waypointVertex.getZ() < 1'000)
			return;

		const auto& runway = rc.getNavigationData().runways[getRunwayIndex()];
		const auto& waypoint = rc.getNavigationData().waypoints[runway.waypointIndex];

		constexpr static uint8_t lineLength = 25;
		constexpr static uint8_t textOffset = 2;

		renderer->renderVerticalLine(
			Point(waypointVertex.getX(), waypointVertex.getY() - lineLength),
			lineLength - 1,
			&Theme::ground2
		);

		renderer->renderFilledCircle(
			Point(waypointVertex.getX(), waypointVertex.getY()),
			2,
			&Theme::ground2
		);

		renderer->renderString(
			Point(
				waypointVertex.getX() - Theme::fontNormal.getWidth(waypoint.name) / 2,
				waypointVertex.getY() - lineLength - textOffset - Theme::fontNormal.getHeight()
			),
			&Theme::fontNormal,
			&Theme::fg1,
			waypoint.name
		);
	}
}

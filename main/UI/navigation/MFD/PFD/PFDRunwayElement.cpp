#include "PFDRunwayElement.h"

#include "rc.h"
#include "UI/theme.h"

namespace pizda {
	PFDRunwayElement::PFDRunwayElement(const uint16_t runwayIndex): RunwayElement(runwayIndex, &Theme::bg1) {

	}

	void PFDRunwayElement::onRender(Renderer* renderer, const Scene& scene, const Vector3F* projectedVertices) {
		RunwayElement::onRender(renderer, scene, projectedVertices);

		const auto& nameVertex = projectedVertices[4];

		if (nameVertex.getZ() < scene.getNearPlaneDistance())
			return;

		const auto& nd = RC::getInstance().getNavigationData();
		const auto& runway = nd.runways[getRunwayIndex()];
		const auto& waypoint = nd.waypoints[runway.waypointIndex];

		constexpr static uint8_t lineLength = 25;
		constexpr static uint8_t textOffset = 2;

		renderer->renderVerticalLine(
			Point(nameVertex.getX(), nameVertex.getY() - lineLength),
			lineLength,
			&Theme::fg1
		);

		renderer->renderString(
			Point(
				nameVertex.getX() - Theme::fontNormal.getWidth(waypoint.name) / 2,
				nameVertex.getY() - lineLength - textOffset - Theme::fontNormal.getHeight()
			),
			&Theme::fontNormal,
			&Theme::fg1,
			waypoint.name
		);
	}
}

#include "routeElement.h"

#include "UI/theme.h"
#include "rc.h"

namespace pizda {
	RouteElement::RouteElement(const uint16_t legIndex) : _legIndex(legIndex) {
		const auto& nd = RC::getInstance().getNavigationData();

		_vertices[0] = nd.waypoints[nd.flightPlan.legs[_legIndex - 1].waypointIndex].cartesianCoordinates;
		_vertices[1] = nd.waypoints[nd.flightPlan.legs[_legIndex].waypointIndex].cartesianCoordinates;
	}

	const Vector3F* RouteElement::getVertices() {
		return _vertices;
	}

	uint16_t RouteElement::getVertexCount() {
		return 2;
	}

	void RouteElement::onRender(Renderer* renderer, const Scene& scene, const Vector3F* projectedVertices) {
		const auto nearPlane = scene.getNearPlaneDistance();

		if (
			projectedVertices[0].getZ() < nearPlane
			|| projectedVertices[1].getZ() < nearPlane
		)
			return;

		const auto& nd = RC::getInstance().getNavigationData();

		renderer->renderLine(
			Point(
				static_cast<int32_t>(projectedVertices[0].getX()),
				static_cast<int32_t>(projectedVertices[0].getY())
			),
			Point(
				static_cast<int32_t>(projectedVertices[1].getX()),
				static_cast<int32_t>(projectedVertices[1].getY())
			),
			_legIndex == nd.flightPlan.activeLegIndex
				? &Theme::magenta1
				: (
					_legIndex < nd.flightPlan.activeLegIndex
					? &Theme::fg3
					: &Theme::fg1
				)
		);
	}
}

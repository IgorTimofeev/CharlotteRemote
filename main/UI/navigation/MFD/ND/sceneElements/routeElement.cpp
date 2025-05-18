#include "routeElement.h"

#include <UI/theme.h>
#include <rc.h>

namespace pizda {
	RouteElement::RouteElement(const NavigationWaypointData* from, const NavigationWaypointData* to, const Color* color) :
		_from(from),
		_to(to),
		_color(color)
	{
		_vertices[0] = from->cartesianCoordinates;
		_vertices[1] = to->cartesianCoordinates;
	}

	const Vector3F* RouteElement::getVertices() {
		return _vertices;
	}

	uint16_t RouteElement::getVertexCount() {
		return 2;
	}

	void RouteElement::onRender(Renderer* renderer, const Scene& scene, const Vector3F* vertices) {
		const auto nearPlane = scene.getNearPlaneDistance();

		if (
			vertices[0].getZ() < nearPlane
			|| vertices[1].getZ() < nearPlane
		)
			return;

		renderer->renderLine(
			Point(
				static_cast<int32_t>(vertices[0].getX()),
				static_cast<int32_t>(vertices[0].getY())
			),
			Point(
				static_cast<int32_t>(vertices[1].getX()),
				static_cast<int32_t>(vertices[1].getY())
			),
			_color
		);
	}
}

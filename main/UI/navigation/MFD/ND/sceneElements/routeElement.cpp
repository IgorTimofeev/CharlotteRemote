#include "routeElement.h"

#include "../../../../theme.h"
#include "../../../../../rc.h"
#include "../ND.h"

namespace pizda {
	RouteElement::RouteElement(const NavigationDataFlightPlanRoute* routeData, const Color* color) : _routeData(routeData), _color(color) {

	}

	const Vector3F* RouteElement::getVertices() {
		return _routeData->cartesianCoordinates;
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

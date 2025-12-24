#include "routeElement.h"

#include <UI/theme.h>
#include "rc.h"

namespace pizda {
	RouteElement::RouteElement(const Vector3F& from, const Vector3F& to, const Color* color) :
		_color(color)
	{
		_vertices[0] = from;
		_vertices[1] = to;
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

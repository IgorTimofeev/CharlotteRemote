#include "waypointElement.h"


#include "../../../../../rc.h"
#include "../../../../theme.h"
#include "../ND.h"

namespace pizda {
	WaypointElement::WaypointElement(const NavigationWaypointData* waypointData) : _waypointData(waypointData) {

	}

	void WaypointElement::onRender(Renderer* renderer, const Scene& scene, const Vector3F* vertices) {
		const auto center = Point(
			static_cast<int32_t>(vertices[0].getX()),
			static_cast<int32_t>(vertices[0].getY())
		);

		switch (_waypointData->type) {
			case NavigationWaypointType::enroute: {
				ND::renderWaypointStar(
					renderer,
					_waypointData,
					center,
					&Theme::fg1
				);

				break;
			}
			case NavigationWaypointType::terminal: {
				renderer->renderTriangle(
					Point(center.getX(), center.getY() - 3),
					Point(center.getX() - 3, center.getY() + 2),
					Point(center.getX() + 3, center.getY() + 2),
					&Theme::fg1
				);

				renderer->renderString(
					Point(center.getX() + 7, center.getY() - 7),
					&Theme::fontSmall,
					&Theme::fg1,
					_waypointData->name
				);

				break;
			}
			case NavigationWaypointType::airport: {
				renderer->renderCircle(
					center,
					4,
					&Theme::ocean
				);

				renderer->renderString(
					Point(center.getX() + 7, center.getY() - 7),
					&Theme::fontSmall,
					&Theme::ocean,
					_waypointData->name
				);

				break;
			}
		}
	}

	const Vector3F* WaypointElement::getVertices() {
		return &_waypointData->cartesianCoordinates;
	}

	uint16_t WaypointElement::getVertexCount() {
		return 1;
	}
}

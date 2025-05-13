#include "waypointElement.h"


#include "../../../../../rc.h"
#include "../../../../theme.h"
#include "../ND.h"

namespace pizda {
	WaypointElement::WaypointElement(const SettingsNavigationWaypoint* waypoint) : _waypoint(waypoint) {
		_vertex = _waypoint->coordinates.toCartesian();
	}

	void WaypointElement::onRender(Renderer* renderer, const Scene& scene, const Vector3F* vertices) {
		const auto center = Point(
			static_cast<int32_t>(vertices[0].getX()),
			static_cast<int32_t>(vertices[0].getY())
		);

		switch (_waypoint->type) {
			case SettingsNavigationWaypointType::enroute: {
				ND::renderWaypointStar(
					renderer,
					_waypoint,
					center,
					&Theme::fg1
				);

				break;
			}
			case SettingsNavigationWaypointType::terminal: {
				renderer->renderTriangle(
					Point(center.getX(), center.getY() - 3),
					Point(center.getX() - 3, center.getY() + 2),
					Point(center.getX() + 3, center.getY() + 2),
					&Theme::fg1
				);

				renderer->renderString(
					Point(center.getX() + 8, center.getY() - 5),
					&Theme::fontSmall,
					&Theme::fg1,
					_waypoint->name
				);

				break;
			}
			case SettingsNavigationWaypointType::airport: {
				renderer->renderCircle(
					center,
					4,
					&Theme::ocean
				);

				renderer->renderString(
					Point(center.getX() + 8, center.getY() - 5),
					&Theme::fontSmall,
					&Theme::ocean,
					_waypoint->name
				);

				break;
			}
		}
	}

	const Vector3F* WaypointElement::getVertices() {
		return &_vertex;
	}

	uint16_t WaypointElement::getVertexCount() {
		return 1;
	}
}

#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include <types/navigationData.h>

namespace pizda {
	using namespace YOBA;

	class RenderingUtils {
		public:
			static void renderWaypoint(Renderer* renderer, const Point& center, const Color* color, const NavigationWaypointData* waypointData) {
				switch (waypointData->type) {
					case NavigationWaypointType::enroute: {
						renderer->renderRectangle(
							Bounds(
								center.getX() - 1,
								center.getY() - 1,
								3,
								3
							),
							color
						);

						renderer->renderHorizontalLine(
							Point(
								center.getX() - 3,
								center.getY()
							),
							2,
							color
						);

						renderer->renderHorizontalLine(
							Point(
								center.getX() + 2,
								center.getY()
							),
							2,
							color
						);

						renderer->renderVerticalLine(
							Point(
								center.getX(),
								center.getY() - 3
							),
							2,
							color
						);

						renderer->renderVerticalLine(
							Point(
								center.getX(),
								center.getY() + 2
							),
							2,
							color
						);
						break;
					}
					case NavigationWaypointType::terminal: {
						renderer->renderTriangle(
							Point(center.getX(), center.getY() - 3),
							Point(center.getX() - 3, center.getY() + 2),
							Point(center.getX() + 3, center.getY() + 2),
							color
						);

						break;
					}
					case NavigationWaypointType::airport: {
						renderer->renderCircle(
							center,
							4,
							color
						);

						break;
					}
				}
			}
	};
}
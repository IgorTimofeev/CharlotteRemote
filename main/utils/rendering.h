#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "types/navigationData.h"

namespace pizda {
	using namespace YOBA;

	class RenderingUtils {
		public:
			static const Color* getWaypointColor(const NavigationWaypointData& waypointData) {
				switch (waypointData.type) {
					case NavigationWaypointType::enroute: {
						return &Theme::magenta;
					}
					default: {
						return &Theme::ocean;
					}
				}
			}

			static void renderWaypointIcon(Renderer* renderer, const Point& center, const Color* color, const NavigationWaypointData& waypointData) {
				switch (waypointData.type) {
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
					case NavigationWaypointType::runway: {
						renderer->renderCircle(
							center,
							4,
							color
						);

						break;
					}
				}
			}

			static void renderWaypointName(Renderer* renderer, const Point& center, const Color* color, const NavigationWaypointData& waypointData) {
				renderer->renderString(
					Point(center.getX() + 7, center.getY() - 7),
					&Theme::fontSmall,
					color,
					waypointData.name
				);
			}
	};
}
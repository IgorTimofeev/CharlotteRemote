#pragma once

#include <format>

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include <UI/theme.h>
#include <types/navigationData.h>
#include <utils/rendering.h>

namespace pizda {
	using namespace YOBA;

	class WaypointItem : public ActiveElement {
		public:
			explicit WaypointItem(const NavigationWaypointData* waypointData) : waypointData(waypointData) {
				setHeight(40);
			}

			const NavigationWaypointData* waypointData;

		protected:
			void onEvent(Event* event) override {
				Element::onEvent(event);

				if (event->getTypeID() == TouchDownEvent::typeID) {
					setCaptured(true);
					setFocused(true);

					setActive(true);
				}
				else if (event->getTypeID() == TouchUpEvent::typeID) {
					setCaptured(false);

					setActive(false);
				}
			}

			void onRender(Renderer* renderer) override {
				const auto& bounds = getBounds();

				constexpr static uint8_t cornerRadius = 3;
				constexpr static uint8_t nameFontScale = 1;

				renderer->renderFilledRectangle(
					bounds,
					cornerRadius,
					isActive() ? &Theme::bg3 : &Theme::bg2
				);

				renderer->renderRectangle(
					bounds,
					cornerRadius,
					isActive() ? &Theme::bg4 : &Theme::bg3
				);

				auto x = bounds.getX() + 10;
				const auto y = bounds.getYCenter();
				const Color* color;

				switch (waypointData->type) {
					case NavigationWaypointType::enroute:
						color = &Theme::purple;

						break;
					case NavigationWaypointType::terminal:
						color = &Theme::purple;

						break;
					default:
						color = &Theme::ocean;

						break;
				}

				RenderingUtils::renderWaypoint(
					renderer,
					Point(x, y),
					color,
					waypointData
				);

				x += 20;

				renderer->renderString(
					Point(
						x,
						y - Theme::fontNormal.getHeight() / 2 * nameFontScale
					),
					&Theme::fontNormal,
					&Theme::fg1,
					waypointData->name,
					nameFontScale
				);

				const auto coordsText = std::format(
					L"{}, {}",
					YOBA::round(toDegrees(waypointData->geographicCoordinates.getLatitude()), 2),
					YOBA::round(toDegrees(waypointData->geographicCoordinates.getLongitude()), 2)
				);

				x = bounds.getX2() - 10 - Theme::fontNormal.getWidth(coordsText);

				renderer->renderString(
					Point(
						x,
						y - Theme::fontNormal.getHeight() / 2
					),
					&Theme::fontNormal,
					&Theme::fg5,
					coordsText
				);
			}
	};
}
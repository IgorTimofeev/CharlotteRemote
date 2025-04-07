#pragma once

#include "src/main.h"
#include "src/ui.h"

#include "../../../../../../vector3.h"
#include "../../../../../../sinAndCos.h"
#include "../../../../../../geographicCoordinates.h"

#include "../../../../../spatial/object.h"
#include "../../../../../spatial/spatialView.h"
#include "../../../../../spatial/camera.h"

#include "../../../../../theme.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class Aircraft : public SpatialObject {
		public:
			Vector3F* getVertices() override {
				return &_position;
			}

			uint16_t getVertexCount() override {
				return 1;
			}

			void onRender(Renderer* renderer, const Bounds& bounds, Camera* camera, const Vector3F* vertices) override {
				if (vertices[0].getZ() < camera->getNearPlaneDistance())
					return;

				constexpr static const uint8_t triangleWidth = 8;
				constexpr static const uint8_t triangleHeight = 6;

				const auto position = Point(
					(int32_t) vertices[0].getX(),
					(int32_t) vertices[0].getY()
				);

				renderer->renderLine(
					position,
					Point(position.getX(), bounds.getY()),
					&Theme::purple
				);

				renderer->renderTriangle(
					position,
					Point(position.getX() - triangleWidth / 2, position.getY() + triangleHeight),
					Point(position.getX() + triangleWidth / 2, position.getY() + triangleHeight),
					&Theme::fg1
				);
			}

			const Vector3F& getPosition() const {
				return _position;
			}

			void setPosition(const Vector3F& position) {
				_position = position;
			}

		private:
			Vector3F _position = GeographicCoordinates(
				yoba::toRadians(60.01449883137194),
				yoba::toRadians(29.702554658332105),
				GeographicCoordinates::equatorialRadiusMeters
			)
			.toCartesian();
	};
}
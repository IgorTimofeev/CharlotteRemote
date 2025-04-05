#pragma once

#include "src/main.h"
#include "src/ui.h"

#include "../../../../theme.h"
#include "../../../../../vector3.h"
#include "../../../../../sinAndCos.h"
#include "../../../../../geographicCoordinates.h"

#include "../../../../spatial/spatialView.h"
#include "../../../../spatial/camera.h"
#include "../../../../spatial/object.h"
#include "../../../../../vector3.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class AircraftObject : public Object {
		public:
			Vector3F* getVertices() override {
				return &_position;
			}

			uint16_t getVertexCount() override {
				return 1;
			}

			void onRender(Renderer* renderer, Camera* camera, const Vector3F* vertices) override {
				if (vertices[0].getZ() < camera->getNearPlaneDistance())
					return;

				constexpr static const uint8_t triangleWidth = 4;
				constexpr static const uint8_t triangleHeight = 3;

				const auto position = Point(
					(int32_t) vertices[0].getX(),
					(int32_t) vertices[0].getY()
				);

				renderer->renderFilledTriangle(
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


	class ND : public SpatialView {
		public:
			ND();

			constexpr static const uint32_t cameraOffsetMinimum = 100;
			constexpr static const uint32_t cameraOffsetMaximum = GeographicCoordinates::equatorialRadiusMeters;

			const GeographicCoordinates& getCameraOffset() const;
			void setCameraOffset(const GeographicCoordinates& value);

		protected:
			void onTick() override;

			void onEvent(Event* event) override;

		private:
			int32_t _pinchLength = 0;
			Point _touchDownPosition;

			GeographicCoordinates _cameraOffset {
				0,
				0,
				500
			};

			AircraftObject _aircraftObject {};
	};
}
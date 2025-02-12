#pragma once

#include "../../../components/yoba/src/main.h"
#include "../../../components/yoba/src/ui.h"

#include "vector3.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class Camera {
		public:
			const Vector3F& getPosition() const {
				return _position;
			}

			void setPosition(const Vector3F& position) {
				_position = position;
			}

			const Vector3F& getRotation() const {
				return _rotation;
			}

			void setRotation(const Vector3F& rotation) {
				_rotation = rotation;
			}

			float getNearPlaneDistance() const {
				return _nearPlaneDistance;
			}

			void setNearPlaneDistance(float nearPlaneDistance) {
				_nearPlaneDistance = nearPlaneDistance;
			}

			float getFarPlaneDistance() const {
				return _farPlaneDistance;
			}

			void setFarPlaneDistance(float farPlaneDistance) {
				_farPlaneDistance = farPlaneDistance;
			}

			float getFieldOfView() const {
				return _fieldOfView;
			}

			void setFieldOfView(float fieldOfView) {
				_fieldOfView = fieldOfView;
			}


		private:
			Vector3F _position;
			Vector3F _rotation;

			float _nearPlaneDistance = 50.f;
			float _farPlaneDistance = 1000.f;

			float _fieldOfView = yoba::toRadians(45);
	};
}
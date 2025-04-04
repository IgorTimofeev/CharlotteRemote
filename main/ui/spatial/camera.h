#pragma once

#include "../../../components/yoba/src/main.h"
#include "../../../components/yoba/src/ui.h"

#include "../../vector3.h"

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

			void setNearPlaneDistance(float value) {
				_nearPlaneDistance = value;
			}

			float getFarPlaneDistance() const {
				return _farPlaneDistance;
			}

			void setFarPlaneDistance(float value) {
				_farPlaneDistance = value;
			}

			float getFOV() const {
				return _FOV;
			}

			void setFOV(float value) {
				_FOV = value;
			}

		private:
			Vector3F _position;
			Vector3F _rotation;
			float _FOV = toRadians(90);

			float _nearPlaneDistance = 0.1f;
			float _farPlaneDistance = 1000.f;
	};
}
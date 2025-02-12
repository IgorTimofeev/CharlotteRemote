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

			float getFov() const {
				return _fov;
			}

			void setFov(float fov) {
				_fov = fov;
			}

		private:
			Vector3F _position;
			Vector3F _rotation;
			float _fov = 90;
	};
}
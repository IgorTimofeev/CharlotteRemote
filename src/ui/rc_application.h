#pragma once

#include "yoba/application.h"

using namespace yoba;

namespace ui {
	class RCApplication : public Application {
		public:
			RCApplication();

			static RCApplication& getInstance();

			void onTick() override;
			void onRender() override;
			void begin() override;

			float getPitch() const;
			void setPitch(float pitch);

			float getRoll() const;
			void setRoll(float roll);

			float getYaw() const;
			void setYaw(float yaw);

			float getAltitude() const;
			void setAltitude(float altitude);

			float getSpeed() const;
			void setSpeed(float speed);

		private:
			float _pitch = 0;
			float _roll = 0;
			float _yaw = 0;
			float _altitude = 237;
			float _speed = 30;
	};
}
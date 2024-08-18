#pragma once

#include "yoba/application.h"
#include "yoba/hardware/potentiometer.h"

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

			float getBaro() const;
			void setBaro(float baro);

		private:
			uint16_t _palette[16];

			float _pitch = 0;
			float _roll = 0;
			float _yaw = 0;
			float _altitude = 237;
			float _speed = 30;
			float _baro = 29.92;

			Potentiometer _pitchHall = Potentiometer(4, 1400, 2860, 20);
			Potentiometer _rollHall = Potentiometer(27, 1290, 3030, 20);
	};
}
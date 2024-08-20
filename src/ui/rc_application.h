#pragma once

#include "yoba/application.h"
#include "yoba/hardware/potentiometer.h"
#include "transceiver/transceiver.h"

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
			void setPitch(float value);

			float getRoll() const;
			void setRoll(float value);

			float getYaw() const;
			void setYaw(float value);

			float getAltitude() const;
			void setAltitude(float value);

			float getSpeed() const;
			void setSpeed(float speed);

			float getPressure() const;
			void setPressure(float value);

			bool isPressureHPA() const;
			void setPressureHPA(bool value);

			bool isPressureSTD() const;
			void setPressureSTD(bool value);

		private:
			uint16_t _palette[16] {};

			float _pitch = 0;
			float _roll = 0;
			float _yaw = 0;
			float _altitude = 237;
			float _speed = 30;
			float _pressure = 1022;
			bool _pressureHPA = true;
			bool _pressureSTD = false;

			Potentiometer _pitchHall;
			Potentiometer _rollHall;

			Transceiver _transceiver;
	};
}
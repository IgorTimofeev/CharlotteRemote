#pragma once

#include <cstdint>
#include "sinAndCos.h"
#include "../components/yoba/src/main.h"

namespace pizda {
	enum class AltimeterMode : uint8_t  {
		QNH,
		QNE
	};

	class LocalData {
		public:
			uint8_t getThrottle() const;
			void setThrottle(uint8_t throttle);

			uint8_t getAilerons() const;
			void setAilerons(uint8_t ailerons);

			uint8_t getRudder() const;
			void setRudder(uint8_t rudder);

			uint8_t getFlaps() const;
			void setFlaps(uint8_t flaps);

			AltimeterMode getAltimeterMode() const;
			void setAltimeterMode(AltimeterMode altimeterMode);

			float getAltimeterPressure() const;
			void setAltimeterPressure(float value);

			bool getStrobeLights() const;
			void setStrobeLights(bool strobeLights);

			float getAutopilotSpeed() const;
			void setAutopilotSpeed(float autopilotSpeed);

			float getAutopilotAltitude() const;
			void setAutopilotAltitude(float autopilotAltitude);

			float getAutopilotHeading() const;
			void setAutopilotHeading(float autopilotHeading);

		private:
			uint8_t _throttle = 0;
			uint8_t _ailerons = 0;
			uint8_t _rudder = 0;
			uint8_t _flaps = 0;

			AltimeterMode _altimeterMode = AltimeterMode::QNH;
			float _altimeterPressure = 1013;

			bool _strobeLights = false;

			float _autopilotSpeed = 0;
			float _autopilotHeading = 0;
			float _autopilotAltitude = 0;
	};

	class RemoteData {
		public:
			float getLatitude() const;
			void setLatitude(float latitude);

			float getLongitude() const;
			void setLongitude(float longitude);

			float getPitch() const;
			void setPitch(float pitch);

			float getRoll() const;
			void setRoll(float roll);

			float getYaw() const;
			void setYaw(float yaw);

			float getTemperature() const;
			void setTemperature(float temperature);

			float getPressure() const;
			void setPressure(float pressure);

			float getAltitude() const;
			void setAltitude(float altitude);

			float getSpeed() const;
			void setSpeed(float speed);

		private:
			// Kronshtadt airfield in Saint-Petersburg for UI testing
			float _latitude = yoba::toRadians(60.01449883137194);
			float _longitude = yoba::toRadians(29.702554658332105);

			float _pitch = 0;
			float _roll = 0;
			float _yaw = 0;

			float _temperature = 30;
			float _pressure = 0;

			float _altitude = 0;
			float _speed = 0;
	};

	class ComputedData {
		public:
			SinAndCos& getLatitudeSinAndCos();
			SinAndCos& getLongitudeSinAndCos();
			SinAndCos& getRollSinAndCos();
			SinAndCos& getPitchSinAndCos();
			SinAndCos& getYawSinAndCos();

		private:
			SinAndCos _latitudeSinAndCos;
			SinAndCos _longitudeSinAndCos;
			SinAndCos _rollSinAndCos;
			SinAndCos _pitchSinAndCos;
			SinAndCos _yawSinAndCos;
	};
}
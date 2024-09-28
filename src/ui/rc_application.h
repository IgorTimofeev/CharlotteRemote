#pragma once

#include "hardware/transceiver/transceiver.h"
#include "hardware/onboard_led.h"
#include "theme.h"
#include "../interpolator.h"
#include "../../lib/YOBA/src/ui/application.h"
#include "../../lib/YOBA/src/hardware/screen/drivers/ILI9341Driver.h"
#include "../../lib/YOBA/src/hardware/screen/buffers/bits8PaletteBuffer.h"
#include "../../lib/YOBA/src/hardware/touch/drivers/FT6336UDriver.h"
#include "../../lib/YOBA/src/color.h"
#include "../../lib/YOBA/src/ui/shapes/rectangle.h"
#include "../../lib/YOBA/src/ui/text.h"
#include "../../lib/YOBA/src/ui/application.h"
#include "../../lib/YOBA/src/ui/debug/touchCanvas.h"
#include "../../lib/YOBA/src/ui/debug/testView.h"
#include "../ui/navigation/menu.h"

using namespace yoba;

namespace pizdanc {
	class MutualData {
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
			void setAltimeterPressure(float altimeterPressure);

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

	class LocalData : public MutualData {

	};

	class RemoteData : public MutualData {
		public:
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
			float _pitch = 0;
			float _roll = 0;
			float _yaw = 0;

			float _temperature = 30;
			float _pressure = 0;

			float _altitude = 0;
			float _speed = 0;
	};

	class RCApplication : public Application {
		public:
			RCApplication();

			static RCApplication& getInstance();

			void setup() override;
			void tick() override;

			LocalData& getLocalData();
			RemoteData& getRemoteData();
			OnboardLED& getOnboardLED();
			Transceiver& getTransceiver();

			Interpolator& getThrottleInterpolator();
			Interpolator& getSpeedInterpolator();
			Interpolator& getAltitudeInterpolator();
			Interpolator& getPitchInterpolator();
			Interpolator& getRollInterpolator();
			Interpolator& getYawInterpolator();
			Interpolator& getSpeedTrendInterpolator();
			Interpolator& getAltitudeTrendInterpolator();
			Interpolator& getVerticalSpeedInterpolator();
			Interpolator& getAileronsInterpolator();
			Interpolator& getFlapsInterpolator();
			Interpolator& getElevatorInterpolator();
			Interpolator& getRudderInterpolator();

		private:
			// -------------------------------- Hardware --------------------------------

			Transceiver _transceiver = Transceiver();

			ILI9341Driver _screenDriver = ILI9341Driver(
				ScreenOrientation::Landscape270,
				settings::pinout::screen::chipSelect,
				settings::pinout::screen::dataCommand,
				settings::pinout::screen::reset
			);

			Bits8PaletteBuffer _screenBuffer = Bits8PaletteBuffer(&_screenDriver);

			FT6336UDriver _touchDriver = FT6336UDriver(
				settings::pinout::screen::touch::reset,
				settings::pinout::screen::touch::interrupt
			);

			OnboardLED _onboardLED = OnboardLED();

			//
//			Potentiometer _pitchHall;
//			Potentiometer _rollHall;

			// -------------------------------- UI --------------------------------

			Menu _menu = Menu();

			// -------------------------------- Timings --------------------------------

			uint32_t _tickTime = 0;
			uint32_t _testTickTime = 0;

			Interpolator _throttleInterpolator = Interpolator();

			Interpolator _speedInterpolator = Interpolator();
			Interpolator _speedTrendInterpolator = Interpolator();

			Interpolator _altitudeInterpolator = Interpolator();
			Interpolator _altitudeTrendInterpolator = Interpolator();

			Interpolator _verticalSpeedInterpolator = Interpolator();

			Interpolator _pitchInterpolator = Interpolator();
			Interpolator _rollInterpolator = Interpolator();
			Interpolator _yawInterpolator = Interpolator();

			Interpolator _aileronsInterpolator = Interpolator();
			Interpolator _flapsInterpolator = Interpolator();
			Interpolator _elevatorInterpolator = Interpolator();
			Interpolator _rudderInterpolator = Interpolator();

			// -------------------------------- Other shit --------------------------------

			LocalData _localData = LocalData();
			RemoteData _remoteData = RemoteData();
	};
}
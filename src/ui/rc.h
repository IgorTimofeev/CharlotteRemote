#pragma once

#include "../../lib/yoba/src/main.h"
#include "../../lib/yoba/src/hardware/hal/arduinoHal.h"
#include "../../lib/yoba/src/hardware/displays/ILI9341Display.h"
#include "../../lib/yoba/src/hardware/touchPanels/FT6336UTouchPanel.h"
#include "../../lib/yoba/src/ui.h"

#include "interpolator.h"

#include "hardware/transceiver/transceiver.h"
#include "hardware/onboard_led.h"

#include "ui/theme.h"
#include "ui/navigation/menu.h"
#include "ui/debugOverlay.h"

namespace pizdanc {
	using namespace yoba;
	using namespace yoba::hardware;
	using namespace yoba::ui;

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

	class RC  {
		public:
			static RC& getInstance();

			void setup();

			void tick();

			Application& getApplication();
			Element& getDebugOverlay();

			LocalData& getLocalData();
			RemoteData& getRemoteData();
			OnboardLED& getOnboardLED();
			Transceiver& getTransceiver();

			Interpolator& getThrottle1Interpolator();
			Interpolator& getThrottle2Interpolator();
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
			Interpolator& getSpoilersInterpolator();
			Interpolator& getElevatorInterpolator();
			Interpolator& getRudderInterpolator();
			Interpolator& getAileronsTrimInterpolator();
			Interpolator& getElevatorTrimInterpolator();
			Interpolator& getRudderTrimInterpolator();

			uint32_t getTickDeltaTime() const;

		private:
			// -------------------------------- Hardware --------------------------------

			hardware::ArduinoHal _hal;

			ILI9341Display _display = ILI9341Display(
				&_hal,
				ColorModel::rgb565,
				RenderTargetOrientation::clockwise0,
				settings::pinout::screen::chipSelect,
				settings::pinout::screen::dataCommand,
				settings::pinout::screen::reset
			);

			Bit8PaletteRenderer _renderer = Bit8PaletteRenderer(32);

			FT6336UTouchPanel _touchPanel = FT6336UTouchPanel(
				&_hal,
				settings::pinout::screen::touch::interrupt,
				settings::pinout::screen::touch::reset,
				settings::pinout::screen::touch::sda,
				settings::pinout::screen::touch::scl
			);

			Transceiver _transceiver;
			OnboardLED _onboardLED;

			//
//			Potentiometer _pitchHall;
//			Potentiometer _rollHall;

			// -------------------------------- UI --------------------------------

			Application _application = Application(&_hal);
			Menu _menu;
			DebugOverlay _debugOverlay;

			// -------------------------------- Timings --------------------------------

			uint32_t _simulationTickTime1 = 0;
			uint32_t _simulationTickTime2 = 0;

			Interpolator _throttle1Interpolator;
			Interpolator _throttle2Interpolator;

			Interpolator _speedInterpolator;
			Interpolator _speedTrendInterpolator;

			Interpolator _altitudeInterpolator;
			Interpolator _altitudeTrendInterpolator;

			Interpolator _verticalSpeedInterpolator;

			Interpolator _pitchInterpolator;
			Interpolator _rollInterpolator;
			Interpolator _yawInterpolator;

			Interpolator _aileronsInterpolator;
			Interpolator _elevatorInterpolator;
			Interpolator _rudderInterpolator;
			Interpolator _flapsInterpolator;
			Interpolator _spoilersInterpolator;

			Interpolator _aileronsTrimInterpolator;
			Interpolator _elevatorTrimInterpolator;
			Interpolator _rudderTrimInterpolator;

			// -------------------------------- Other shit --------------------------------

			LocalData _localData;
			RemoteData _remoteData;
			uint32_t _tickDeltaTime;

			void simulateFlightData();
	};
}
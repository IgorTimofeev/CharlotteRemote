#pragma once



#include "src/main.h"
#include "src/ui.h"
#include "src/hardware/displays/ILI9341Display.h"
#include "src/hardware/touchPanels/FT6336UTouchPanel.h"

#include "settings.h"
#include "constants.h"
#include "data.h"
#include "interpolator.h"
#include "hardware/transceiver/transceiver.h"

#include "ui/theme.h"
#include "ui/navigation/menu.h"
#include "ui/debugOverlay.h"
#include "hardware/battery.h"

namespace pizdanc {
	using namespace yoba;
	using namespace yoba::hardware;
	using namespace yoba::ui;

	class RC {
		public:
			static RC& getInstance();

			void run();

			Application& getApplication();

			LocalData& getLocalData();
			RemoteData& getRemoteData();

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

			void updateDebugInfoVisibility();
			uint32_t getTickDeltaTime() const;
			Settings& getSettings();

			const Battery& getBattery() const;

		private:
			RC() = default;

			// -------------------------------- Hardware --------------------------------

			ILI9341Display _display = ILI9341Display(
				constants::pinout::screen::mosi,
				constants::pinout::screen::miso,
				constants::pinout::screen::sck,
				constants::pinout::screen::chipSelect,
				constants::pinout::screen::dataCommand,
				constants::pinout::screen::reset,
				constants::pinout::screen::frequency
			);

			EightBitPaletteRenderer _renderer = EightBitPaletteRenderer(32);

			FT6336UTouchPanel _touchPanel = FT6336UTouchPanel(
				constants::pinout::screen::touch::sda,
				constants::pinout::screen::touch::scl,
				constants::pinout::screen::touch::reset,
				constants::pinout::screen::touch::interrupt
			);

			//
//			Potentiometer _pitchHall;
//			Potentiometer _rollHall;


			/**
			Some thoughts about measuring voltage & charge in percents using ADC:

			1) Safe voltage range for Li-ion 18650 battery is [2.5; 4.2]V, and for 2x batteries
			in series it escalates to [5.0; 8.4]V. But let's give it some safety margins like
			[6.0; 8.4]V, because of tons of trash batteries on market

			2) In theory ADC should read up to 3.3V from GPIO, but Espressif docs says that ADC
			configured with 12 dB attenuation can accurately measure only [0.15; 2.45]V on ESP32
			See: https://docs.espressif.com/projects/esp-idf/en/release-v4.3/esp32/api-reference/peripherals/adc.html

			Based on this shit & resistors I have, the voltage divider will be 680R / 330R,
			giving final input range of [1.96; 2.74]V. This will slightly exceed the 2.45V
			ADC limit, but there's no any risks while we're staying below 3.3V. ADC will simple
			interpret such "higher" values as "max values"
			*/
			Battery _battery = Battery(
				constants::pinout::batteryVoltage,
				1960,
				2744
			);

			// -------------------------------- UI --------------------------------

			Application _application;
			Menu _menu;
			DebugOverlay _debugOverlay;

			// -------------------------------- Timings --------------------------------

			uint32_t _tickDeltaTime = 0;

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

			Settings _settings;
			LocalData _localData;
			RemoteData _remoteData;

			void simulateFlightData();
	};
}
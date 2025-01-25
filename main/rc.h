#pragma once

#include "esp_adc/adc_oneshot.h"

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
			float getBatteryCharge() const;
			uint32_t getTickDeltaTime() const;
			Settings& getSettings();

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

			// -------------------------------- UI --------------------------------

			Application _application;
			Menu _menu;
			DebugOverlay _debugOverlay;

			// -------------------------------- Battery --------------------------------

			static const uint8_t _batteryVoltageMaxCount = 16;
			static const uint32_t _batteryVoltageInterval = 1000000 / _batteryVoltageMaxCount;
			const float _batteryVoltageDividerMin = 1.83f;
			const float _batteryVoltageDividerMax = 2.89f;

			adc_oneshot_unit_handle_t _batteryVoltageADCHandle;
			uint32_t _batteryVoltageTime = 0;
			uint32_t _batteryVoltageSum = 0;
			uint8_t _batteryVoltageCount = 0;
			float _batteryCharge = 0.0f;

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

			void readBatteryVoltage();
	};
}
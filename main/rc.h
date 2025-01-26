#pragma once

#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"

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
			uint16_t getBatteryCharge() const;
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

			/**
			Some thoughts about measuring voltage & charge in percents using ADC:

			1) "Safe" voltage range for Li-ion 18650 battery is [2.6; 4.2]V, so for 2x batteries
			in series the range becomes [5.2; 8.4]V

			2) In theory ADC should read up to 3.3V from GPIO, but Espressif docs says that ADC
			configured with ADC_ATTEN_DB_12 can accurately measure only [0.15; 2.45]V on ESP32
			See: https://docs.espressif.com/projects/esp-idf/en/release-v4.3/esp32/api-reference/peripherals/adc.html

			Based on this shit & resistors I have, the voltage divider will be 680R / 330R,
		 	giving final working range of [1,69; 2.74]V
			*/

			// 8 samples per second should be more than enough
			static constexpr uint8_t _batteryADCSampleCount = 8;
			static constexpr uint32_t _batteryADCSamplingInterval = 1000000 / _batteryADCSampleCount;

			static constexpr uint16_t _batteryVoltageDividerMin = 1830;
			static constexpr uint16_t _batteryVoltageDividerMax = 2540;
			static constexpr uint16_t _batteryVoltagePinMax = 3300;

			static constexpr uint16_t _batteryADCMax = 4095;
			static constexpr uint16_t _batteryADCVoltageMin = _batteryVoltageDividerMin * _batteryADCMax / _batteryVoltagePinMax;
			static constexpr uint16_t _batteryADCVoltageMax = _batteryVoltageDividerMax * _batteryADCMax / _batteryVoltagePinMax;

			static constexpr uint16_t _batteryChargeMax = std::numeric_limits<uint16_t>::max();

			adc_cali_handle_t _batteryADCCaliHandle;
			adc_oneshot_unit_handle_t _batteryADCHandle;
			uint32_t _batteryADCSampleTime = 0;
			uint32_t _batteryADCSampleSum = 0;
			uint8_t _batteryADCSampleIndex = 0;
			uint16_t _batteryCharge = 0;

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

			void batteryTick();

			void batterySetup();
	};
}
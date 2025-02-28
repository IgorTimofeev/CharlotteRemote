#pragma once


#include "src/main.h"
#include "src/ui.h"
#include "src/hardware/displays/ILI9341Display.h"
#include "src/hardware/touchPanels/FT6336UTouchPanel.h"

#include "settings.h"
#include "constants.h"
#include "data.h"
#include "interpolator.h"

#include "ui/theme.h"
#include "ui/navigation/tabBar.h"
#include "ui/debugOverlay.h"

#include "hardware/transceiver/transceiver.h"
#include "hardware/speaker.h"
#include "hardware/analog.h"
#include "hardware/battery.h"
#include "hardware/encoder.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::hardware;
	using namespace yoba::ui;

	class RC {
		public:
			static RC& getInstance();

			[[noreturn]] void run();

			Application& getApplication();

			LocalData& getLocalData();
			RemoteData& getRemoteData();
			ComputedData& getComputedData();

			Interpolator& getSpeedInterpolator();
			Interpolator& getAltitudeInterpolator();
			Interpolator& getPitchInterpolator();
			Interpolator& getRollInterpolator();
			Interpolator& getYawInterpolator();
			Interpolator& getSpeedTrendInterpolator();
			Interpolator& getAltitudeTrendInterpolator();
			Interpolator& getVerticalSpeedInterpolator();
			Interpolator& getAileronsInterpolator();
			Interpolator& getElevatorInterpolator();
			Interpolator& getRudderInterpolator();
			Interpolator& getAileronsTrimInterpolator();
			Interpolator& getElevatorTrimInterpolator();
			Interpolator& getRudderTrimInterpolator();

			void updateDebugInfoVisibility();
			uint32_t getTickDeltaTime() const;
			Settings& getSettings();

			Speaker& getSpeaker();

			Analog& getLeverLeft();
			Encoder& getEncoder();
			Analog& getLeverRight();
			Analog& getJoystickHorizontal();
			Analog& getJoystickVertical();
			Analog& getRing();
			Battery& getBattery();

		private:
			RC() = default;

			// -------------------------------- Hardware --------------------------------

			adc_oneshot_unit_handle_t _ADC1UnitHandle {};

			ILI9341Display _display = ILI9341Display(
				constants::hardware::spi::mosi,
				constants::hardware::spi::miso,
				constants::hardware::spi::sck,

				constants::hardware::screen::slaveSelect,
				constants::hardware::screen::dataCommand,
				constants::hardware::screen::reset,
				constants::hardware::screen::frequency
			);

			EightBitPaletteRenderer _renderer = EightBitPaletteRenderer(32);

			FT6336UTouchPanel _touchPanel = FT6336UTouchPanel(
				constants::hardware::i2c::sda,
				constants::hardware::i2c::scl,

				constants::hardware::screen::touch::reset,
				constants::hardware::screen::touch::interrupt
			);

			Speaker _speaker {};
			Transceiver _transceiver {};

			Analog _leverLeft = Analog(&_ADC1UnitHandle, ADC_CHANNEL_0);
			Encoder _encoder = Encoder(GPIO_NUM_25, GPIO_NUM_26, GPIO_NUM_27);
			Analog _leverRight = Analog(&_ADC1UnitHandle, ADC_CHANNEL_3);

			Analog _joystickHorizontal = Analog(&_ADC1UnitHandle, ADC_CHANNEL_7);
			Analog _joystickVertical = Analog(&_ADC1UnitHandle, ADC_CHANNEL_5);
			Analog _ring = Analog(&_ADC1UnitHandle, ADC_CHANNEL_6);

			/**
			Some thoughts about measuring voltage & charge in percents using ADC:

			1) Safe voltage range for Li-ion 18650 battery is [2.5; 4.2]V, and for 2x batteries
			in series it escalates to [5.0; 8.4]V. But let's give it some safety margins like
			[6.0; 8.4]V, because of tons of trash batteries on market

			2) In theory ADC should read up to 3.3V from GPIO, but Espressif docs says that ADC
			configured with 12 dB attenuation can accurately measure only [0.15; 2.45]V on ESP32
			See: https://docs.espressif.com/projects/esp-idf/en/release-v4.3/esp32/api-reference/peripherals/adc.html

			Based on this shit & resistors I have, the voltage divider will be 1M / 330K,
			giving final input range of [1,488; 2.084]V
			*/
			Battery _battery = Battery(
				&_ADC1UnitHandle,
				ADC_CHANNEL_4,
				6000,
				7400,
				1000000,
				330000
			);

			// -------------------------------- UI --------------------------------

			Application _application;
			TabBar _tabBar;
			DebugOverlay _debugOverlay;

			// -------------------------------- Timings --------------------------------

			uint32_t _tickDeltaTime = 0;

			uint32_t _simulationTickTime1 = 0;
			uint32_t _simulationTickTime2 = 0;

			constexpr static const uint32_t _inputDevicesTickInterval = 1000000 / 30;
			uint32_t _inputDevicesTickTime = 0;

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

			Interpolator _aileronsTrimInterpolator;
			Interpolator _elevatorTrimInterpolator;
			Interpolator _rudderTrimInterpolator;

			// -------------------------------- Other shit --------------------------------

			Settings _settings;
			LocalData _localData;
			RemoteData _remoteData;
			ComputedData _computedData;

			void simulateFlightData();

			void updateComputedData();

			static void SPIBusSetup();
			void ADCUnitsSetup();

			void inputDevicesTick();
	};
}
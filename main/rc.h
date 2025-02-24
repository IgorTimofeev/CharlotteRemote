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
#include "hardware/slave.h"

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

			Speaker& getSpeaker();
			Slave& getSlave();

		private:
			RC() = default;

			// -------------------------------- Hardware --------------------------------

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
				constants::hardware::screen::touch::sda,
				constants::hardware::screen::touch::scl,
				constants::hardware::screen::touch::reset,
				constants::hardware::screen::touch::interrupt
			);

			//
//			Potentiometer _pitchHall;
//			Potentiometer _rollHall;

			Speaker _speaker {};
			Slave _slave {};
			Transceiver _transceiver {};

			// -------------------------------- UI --------------------------------

			Application _application;
			TabBar _tabBar;
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
			ComputedData _computedData;

			void simulateFlightData();

			void updateComputedData();

			void SPIBusSetup();

			void fromSlave();
	};
}
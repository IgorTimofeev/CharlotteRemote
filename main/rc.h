#pragma once

#include "src/main.h"
#include "src/ui.h"
#include "src/hardware/displays/ILI9341Display.h"
#include "src/hardware/touchPanels/FT6336UTouchPanel.h"

#include "data.h"
#include "interpolator.h"
#include "settings.h"
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
			DebugOverlay& getDebugOverlay();

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

			uint32_t getTickDeltaTime() const;

		private:
			RC() = default;

			// -------------------------------- Hardware --------------------------------

			ILI9341Display _display = ILI9341Display(
				settings::pinout::screen::mosi,
				settings::pinout::screen::miso,
				settings::pinout::screen::sck,
				settings::pinout::screen::chipSelect,
				settings::pinout::screen::dataCommand,
				settings::pinout::screen::reset,
				settings::pinout::screen::frequency
			);

			EightBitPaletteRenderer _renderer = EightBitPaletteRenderer(32);

			FT6336UTouchPanel _touchPanel = FT6336UTouchPanel(
				settings::pinout::screen::touch::sda,
				settings::pinout::screen::touch::scl,
				settings::pinout::screen::touch::reset,
				settings::pinout::screen::touch::interrupt
			);

			//
//			Potentiometer _pitchHall;
//			Potentiometer _rollHall;

			// -------------------------------- UI --------------------------------

			Application _application = Application();
			Menu _menu = Menu();
			DebugOverlay _debugOverlay = DebugOverlay();

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
			uint32_t _tickDeltaTime = 0;

			void simulateFlightData();
	};
}
#pragma once

#include <esp_log.h>
#include <esp_adc/adc_oneshot.h>

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include <YOBA/hardware/displays/ILI9341Display.h>
#include <YOBA/hardware/touchPanels/FT6336UTouchPanel.h>
#include <YOBA/hardware/encoder.h>

#include <units.h>
#include <battery.h>
#include <lowPassFilter.h>
#include <audioPlayer.h>

#include "config.h"
#include "UI/theme.h"
#include "UI/navigation/route.h"
#include "UI/navigation/menu/openMenuButton.h"
#include "UI/elements/debugOverlay.h"

#include "config.h"
#include "settings/settings.h"

#include "systems/transceiver/transceiver.h"
#include "systems/transceiver/remoteTransceiver.h"
#include "systems/axes/axes.h"

#include "types/remoteData.h"
#include "types/remoteData.h"
#include "types/aircraftData.h"
#include "types/navigationData.h"

namespace pizda {
	using namespace YOBA;

	class RC {
		public:
			static RC& getInstance();

			[[noreturn]] void run();

			Application& getApplication();
			Settings& getSettings();
			AudioPlayer& getAudioPlayer();
			PushButtonEncoder& getEncoder();
			Axes& getAxes();

			Battery getBattery() const;
			RemoteTransceiver& getTransceiver();
			
			void updateDebugOverlayVisibility();

			const Route* getRoute() const;
			void setRoute(const Route* route);
			
			RemoteData& getRemoteData();
			AircraftData& getAircraftData();
			NavigationData& getNavigationData();

			SemaphoreHandle_t getSPIMutex() const;

			void playFeedback(const Sound* sound);
			void playFeedback();

			constexpr adc_oneshot_unit_handle_t* getAssignedADCOneshotUnit(const adc_unit_t ADCUnit) {
				switch (ADCUnit) {
					case ADC_UNIT_1: return &_ADCOneshotUnit1;
					default: startErrorLoop("failed to find assigned ADC oneshot unit");
				}
			}

		private:
			constexpr static const char* _logTag = "RC";
			
			RC() = default;

			// -------------------------------- Multicore --------------------------------

			SemaphoreHandle_t _SPIMutex = nullptr;

			// -------------------------------- Hardware --------------------------------

			adc_oneshot_unit_handle_t _ADCOneshotUnit1 {};

			ILI9341Display _display {
				config::spi::MOSI,
				config::spi::MISO,
				config::spi::SCK,

				config::screen::SS,
				config::screen::DC,
				config::screen::RST,
				config::screen::frequency
			};

			Bit8PaletteRenderer _renderer { 32 };

			FT6336UTouchPanel _touchPanel {
				config::i2c::SDA,
				config::i2c::SCL,
				config::screen::touch::RST,
				config::screen::touch::INTR
			};

			// Transceiver
			RemoteTransceiver _transceiver {};
			
			// Encoder
			PushButtonEncoder _encoder {
				config::encoder::a,
				config::encoder::b,
				config::encoder::sw
			};

			// Analog axes
			Axes _axes {};

			// Battery
			Battery _battery {
				config::battery::remote::unit,
				getAssignedADCOneshotUnit(config::battery::remote::unit),
				config::battery::remote::channel,

				config::battery::remote::voltageMin,
				config::battery::remote::voltageMax,
				config::battery::remote::voltageDividerR1,
				config::battery::remote::voltageDividerR2
			};

			// Audio
			Buzzer _buzzer {
				config::buzzer::gpio,
				config::buzzer::channel
			};

			AudioPlayer _audioPlayer { &_buzzer };

			// -------------------------------- UI --------------------------------

			Application _application {};
			Layout _pageLayout {};

			OpenMenuButton _openMenuButton {};
			DebugOverlay* _debugOverlay = nullptr;

			const Route* _route = nullptr;

			// -------------------------------- Other shit --------------------------------

			Settings _settings;
			RemoteData _remoteData {};
			AircraftData _aircraftData {};
			NavigationData _navigationData {};

			int64_t _dataInterpolationTime = 0;
			int64_t _batteryTickTime = 0;

			float applyLPF(float oldValue, float newValue, float factor) const;
			float applyLPFToAngle(float oldValue, float newValue, float factor) const;
			void interpolateData();
			void batteryTick();

			[[noreturn]] static void startErrorLoop(const char* error);
	};
}
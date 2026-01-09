#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include <YOBA/hardware/displays/ILI9341Display.h>
#include <YOBA/hardware/touchPanels/FT6336UTouchPanel.h>
#include <YOBA/hardware/encoder.h>

#include <units.h>

#include "UI/theme.h"
#include "UI/navigation/route.h"
#include "UI/navigation/menu/openMenuButton.h"
#include "UI/elements/debugOverlay.h"

#include "config.h"
#include "types/settings/settings.h"

#include "hardware/transceiver/packet.h"
#include "hardware/transceiver/remotePacketHandler.h"
#include "hardware/transceiver/transceiver.h"
#include "hardware/audio/audioPlayer.h"
#include "hardware/axis.h"
#include "hardware/battery.h"

#include "types/remoteData.h"
#include "types/remoteData.h"
#include "types/aircraftData.h"
#include "types/navigationData.h"

#include "utils/lowPassFilter.h"

namespace pizda {
	using namespace YOBA;

	class RC {
		public:
			static RC& getInstance();

			[[noreturn]] void run();

			Application& getApplication();

			Settings& getSettings();

			AudioPlayer& getAudioPlayer();

			Axis& getLeverLeft();
			PushButtonEncoder& getEncoder();
			Axis& getLeverRight();
			Axis& getJoystickHorizontal();
			Axis& getJoystickVertical();
			Axis& getRing();
			Battery& getBattery();
			SX1262Transceiver& getTransceiver();
			RemotePacketHandler& getPacketHandler();
			
			OpenMenuButton& getOpenMenuButton();

			void updateDebugOverlayVisibility();

			const Route* getRoute() const;
			void setRoute(const Route* route);
			
			RemoteData& getRemoteData();
			AircraftData& getAircraftData();
			NavigationData& getNavigationData();
			
			uint32_t getTickDeltaTime() const;
		
		private:
			constexpr static const char* _logTag = "Main";
			
			RC() = default;

			Settings _settings;

			// -------------------------------- Hardware --------------------------------

			ILI9341Display _display = ILI9341Display(
				config::spi::MOSI,
				config::spi::MISO,
				config::spi::SCK,

				config::screen::SS,
				config::screen::DC,
				config::screen::RST,
				config::screen::frequency
			);

			Bit8PaletteRenderer _renderer { 32 };

			FT6336UTouchPanel _touchPanel = FT6336UTouchPanel(
				config::i2c::SDA,
				config::i2c::SCL,

				config::screen::touch::RST,
				config::screen::touch::INTR
			);
			
			AudioPlayer _audioPlayer {};
			
			// Transceiver
			SX1262Transceiver _transceiver {};
			RemotePacketHandler _packetHandler {};
			
			// Encoder
			PushButtonEncoder _encoder {
				config::encoder::a,
				config::encoder::b,
				config::encoder::sw
			};

			// Axis
			int64_t _axisTickTimeUs = 0;

			Axis _leverLeft {
				&config::adc::oneshotUnit,
				config::axis::leverLeft::channel,
				config::axis::leverLeft::invertInput,
				&_settings.axis.leverLeft
			};

			Axis _leverRight {
				&config::adc::oneshotUnit,
				config::axis::leverRight::channel,
				config::axis::leverRight::invertInput,
				&_settings.axis.leverRight
			};

			Axis _joystickHorizontal {
				&config::adc::oneshotUnit,
				config::axis::joystickHorizontal::channel,
				config::axis::joystickHorizontal::invertInput,
				&_settings.axis.joystickHorizontal
			};

			Axis _joystickVertical {
				&config::adc::oneshotUnit,
				config::axis::joystickVertical::channel,
				config::axis::joystickVertical::invertInput,
				&_settings.axis.joystickVertical
			};

			Axis _ring {
				&config::adc::oneshotUnit,
				config::axis::ring::channel,
				config::axis::ring::invertInput,
				&_settings.axis.ring
			};

			Battery _battery {
				config::adc::unit,
				&config::adc::oneshotUnit,
				config::battery::channel,
				
				config::battery::voltageMin,
				config::battery::voltageMax,
				
				config::battery::voltageDividerR1,
				config::battery::voltageDividerR2
			};

			// -------------------------------- UI --------------------------------

			Application _application {};
			Layout _pageLayout {};

			OpenMenuButton _openMenuButton {};
			DebugOverlay* _debugOverlay = nullptr;

			const Route* _route = nullptr;

			// -------------------------------- Other shit --------------------------------
			
			RemoteData _remoteData {};
			AircraftData _aircraftData {};
			
			NavigationData _navigationData {};
			uint32_t _tickDeltaTime = 0;
			
			int64_t _interpolationTickTime = 0;
			
			void SPIBusSetup() const;
			void GPIOSetup() const;
			static void ADCUnitsSetup();

			void axisTick();

			void NVSSetup();
			
			void startErrorLoop(const char* error);
			
			float applyLPF(float oldValue, float newValue, float factor);
			float applyLPFForAngleRad(float oldValue, float newValue, float factor);
			
			void interpolationTick();
	};
}
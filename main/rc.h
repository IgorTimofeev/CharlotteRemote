#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include <YOBA/hardware/displays/ILI9341Display.h>
#include <YOBA/hardware/touchPanels/FT6336UTouchPanel.h>
#include <YOBA/hardware/encoder.h>

#include "UI/theme.h"
#include "UI/navigation/route.h"
#include "UI/navigation/menu/openMenuButton.h"
#include "UI/elements/debugOverlay.h"

#include "config.h"
#include "settings/settings.h"

#include "hardware/transceiver/packet.h"
#include "hardware/transceiver/remotePacketHandler.h"
#include "hardware/transceiver/transceiver.h"
#include "hardware/speaker/speaker.h"
#include "hardware/axis.h"
#include "hardware/battery.h"

#include "units.h"
#include "types/aircraftData.h"
#include "types/navigationData.h"
#include "types/statistics.h"

#include "utils/lowPassFilter.h"

namespace pizda {
	using namespace YOBA;

	class RC {
		public:
			static RC& getInstance();

			[[noreturn]] void run();

			Application& getApplication();

			Settings& getSettings();

			Speaker& getSpeaker();

			Axis& getLeverLeft();
			PushButtonEncoder& getEncoder();
			Axis& getLeverRight();
			Axis& getJoystickHorizontal();
			Axis& getJoystickVertical();
			Axis& getRing();
			Battery& getBattery();
			Transceiver& getTransceiver();
			
			OpenMenuButton& getOpenMenuButton();

			void updateDebugOverlayVisibility();

			const Route* getRoute() const;
			void setRoute(const Route* route);

			AircraftData& getAircraftData();
			NavigationData& getNavigationData();
			Statistics& getStatistics();

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

			Speaker _speaker {};
			
			// Transceiver
			Transceiver _transceiver {};
			RemotePacketHandler _packetHandler {};
			
			// Encoder
			PushButtonEncoder _encoder {
				config::encoder::a,
				config::encoder::b,
				config::encoder::sw
			};

			// Axis
			int64_t _axisTickTimeUs = 0;

			Axis _leverLeft = Axis(
				&config::adc::oneshotUnit,
				config::axis::leverLeft::channel,
				&_settings.axis.leverLeft
			);

			Axis _leverRight = Axis(
				&config::adc::oneshotUnit,
				config::axis::leverRight::channel,
				&_settings.axis.leverRight
			);

			Axis _joystickHorizontal = Axis(
				&config::adc::oneshotUnit,
				config::axis::joystickHorizontal::channel,
				&_settings.axis.joystickHorizontal
			);

			Axis _joystickVertical = Axis(
				&config::adc::oneshotUnit,
				config::axis::joystickVertical::channel,
				&_settings.axis.joystickVertical
			);

			Axis _ring = Axis(
				&config::adc::oneshotUnit,
				config::axis::ring::channel,
				&_settings.axis.ring
			);

			Battery _battery = Battery(
				config::adc::unit,
				&config::adc::oneshotUnit,
				config::battery::channel,

				config::battery::voltageMin,
				config::battery::voltageMax,

				config::battery::voltageDividerR1,
				config::battery::voltageDividerR2
			);

			// -------------------------------- UI --------------------------------

			Application _application {};
			Layout _pageLayout {};

			OpenMenuButton _openMenuButton {};
			DebugOverlay* _debugOverlay = nullptr;

			const Route* _route = nullptr;

			// -------------------------------- Other shit --------------------------------

			AircraftData _aircraftData {};
			NavigationData _navigationData {};
			Statistics _statistics {};
			
			int64_t _interpolationTickTime = 0;
			
			void SPIBusSetup() const;
			void GPIOSetup() const;
			static void ADCUnitsSetup();

			void axisTick();

			void NVSSetup();
			
			void startErrorLoop(const char* error);

			void interpolationTick();
	};
}
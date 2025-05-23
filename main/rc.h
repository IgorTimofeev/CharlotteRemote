#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include <YOBA/hardware/displays/ILI9341Display.h>
#include <YOBA/hardware/touchPanels/FT6336UTouchPanel.h>
#include <YOBA/hardware/rotaryEncoder.h>

#include <UI/theme.h>
#include <UI/navigation/route.h>
#include <UI/navigation/menu/menu.h>
#include <UI/navigation/menu/openMenuButton.h>
#include <UI/elements/debugOverlay.h>

#include <settings/settings.h>
#include <constants.h>

#include <hardware/WiFi/WiFi.h>
#include <hardware/transceiver/packet.h>
#include <hardware/transceiver/TCPTransceiver.h>
#include <hardware/speaker/speaker.h>
#include <hardware/axis.h>
#include <hardware/battery.h>

#include <types/units.h>
#include <types/aircraftData.h>
#include <types/navigationData.h>

#include <utils/lowPassFilter.h>


namespace pizda {
	using namespace YOBA;

	class RC {
		public:
			static RC& getInstance();

			[[noreturn]] void run();

			Application& getApplication();

			uint32_t getTickDeltaTime() const;
			Settings& getSettings();

			Speaker& getSpeaker();

			Axis& getLeverLeft();
			RotaryEncoder& getEncoder();
			Axis& getLeverRight();
			Axis& getJoystickHorizontal();
			Axis& getJoystickVertical();
			Axis& getRing();
			Battery& getBattery();
			TCPTransceiver& getTransceiver();

			void handleAircraftPacket(const AircraftPacket* packet);

			bool isMenuVisible() const;
			void showMenu();
			void hideMenu();
			OpenMenuButton& getOpenMenuButton();

			bool isDebugOverlayVisible();
			void updateDebugOverlayVisibility();

			const Route* getRoute() const;
			void setRoute(const Route* route);

			AircraftData& getAircraftData();
			NavigationData& getNavigationData();

		private:
			RC() = default;

			Settings _settings;

			// -------------------------------- Hardware --------------------------------

			ILI9341Display _display = ILI9341Display(
				constants::spi::mosi,
				constants::spi::miso,
				constants::spi::sck,

				constants::screen::slaveSelect,
				constants::screen::dataCommand,
				constants::screen::reset,
				constants::screen::frequency
			);

			Bit8PaletteRenderer _renderer = Bit8PaletteRenderer(32);

			FT6336UTouchPanel _touchPanel = FT6336UTouchPanel(
				constants::i2c::sda,
				constants::i2c::scl,

				constants::screen::touch::reset,
				constants::screen::touch::interrupt
			);

			Speaker _speaker {};
			TCPTransceiver _transceiver {};

			// Encoder
			RotaryEncoder _encoder = {
				constants::encoder::a,
				constants::encoder::b,
				constants::encoder::sw
			};

			// Axis
			uint32_t _axisTickTime = 0;

			Axis _leverLeft = Axis(
				&constants::adc::oneshotUnit,
				constants::axis::leverLeft::channel,
				&_settings.axis.leverLeft
			);

			Axis _leverRight = Axis(
				&constants::adc::oneshotUnit,
				constants::axis::leverRight::channel,
				&_settings.axis.leverRight
			);

			Axis _joystickHorizontal = Axis(
				&constants::adc::oneshotUnit,
				constants::axis::joystickHorizontal::channel,
				&_settings.axis.joystickHorizontal
			);

			Axis _joystickVertical = Axis(
				&constants::adc::oneshotUnit,
				constants::axis::joystickVertical::channel,
				&_settings.axis.joystickVertical
			);

			Axis _ring = Axis(
				&constants::adc::oneshotUnit,
				constants::axis::ring::channel,
				&_settings.axis.ring
			);

			Battery _battery = Battery(
				constants::adc::unit,
				&constants::adc::oneshotUnit,
				constants::battery::channel,

				constants::battery::voltageMin,
				constants::battery::voltageMax,

				constants::battery::voltageDividerR1,
				constants::battery::voltageDividerR2
			);

			// -------------------------------- UI --------------------------------

			Application _application {};
			Layout _pageLayout {};

			OpenMenuButton _openMenuButton {};
			Menu* _menu = nullptr;
			DebugOverlay* _debugOverlay = nullptr;

			const Route* _route = nullptr;

			// -------------------------------- Timings --------------------------------

			uint32_t _tickDeltaTime = 0;
			uint32_t _interpolationTickTime = 0;
			uint32_t _aircraftPacketTime = 0;

			// -------------------------------- Other shit --------------------------------

			AircraftData _aircraftData {};
			NavigationData _navigationData {};

			void SPIBusSetup() const;
			static void ADCUnitsSetup();

			void axisTick();

			void NVSSetup();

			void interpolationTick();
	};
}
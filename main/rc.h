#pragma once


#include "../../components/yoba/src/main.h"
#include "../../components/yoba/src/ui.h"
#include "../../components/yoba/src/hardware/displays/ILI9341Display.h"
#include "../../components/yoba/src/hardware/touchPanels/FT6336UTouchPanel.h"

#include "ui/theme.h"
#include "ui/navigation/routing.h"
#include "ui/navigation/menu.h"
#include "ui/debugOverlay.h"

#include "types.h"
#include "settings.h"
#include "constants.h"
#include "interpolator.h"
#include "vector3.h"
#include "geocentricCoordinates.h"

#include "hardware/transceiver/packet.h"
#include "hardware/transceiver/TCPTransceiver.h"
#include "hardware/speaker.h"
#include "hardware/axis.h"
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

			LowPassInterpolator& getAirspeedInterpolator();
			LowPassInterpolator& getAltitudeInterpolator();
			LowPassInterpolator& getPitchInterpolator();
			LowPassInterpolator& getRollInterpolator();
			LowPassInterpolator& getYawInterpolator();
			LowPassInterpolator& getAirspeedTrendInterpolator();
			LowPassInterpolator& getAltitudeTrendInterpolator();
			LowPassInterpolator& getVerticalSpeedInterpolator();
			LowPassInterpolator& getSlipAndSkidInterpolator();

			float getAltimeterPressure() const;
			void setAltimeterPressure(float altimeterPressure);

			uint32_t getTickDeltaTime() const;
			Settings& getSettings();

			Speaker& getSpeaker();

			Axis& getLeverLeft();
			Encoder& getEncoder();
			Axis& getLeverRight();
			Axis& getJoystickHorizontal();
			Axis& getJoystickVertical();
			Axis& getRing();
			Battery& getBattery();

			void handleAircraftPacket(AircraftPacket* packet);

			bool isMenuVisible();

			void setMenuVisibility(bool state);

			bool isDebugOverlayVisible();
			void setDebugOverlayVisibility(bool state);

			const Route* getRoute();
			void setRoute(const Route* route);

			const GeocentricCoordinates& getGeocentricCoordinates() const;

			const Vector3F& getFlightPathVector() const;

			const Vector2F& getFlightPathAngles() const;

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

			EightBitPaletteRenderer _renderer = EightBitPaletteRenderer(32);

			FT6336UTouchPanel _touchPanel = FT6336UTouchPanel(
				constants::i2c::sda,
				constants::i2c::scl,

				constants::screen::touch::reset,
				constants::screen::touch::interrupt
			);

			Speaker _speaker;
			TCPTransceiver _transceiver;

			// Encoder
			uint32_t _encoderRotationTime = 0;
			bool _encoderWasPressed = false;

			Encoder _encoder = Encoder(
				constants::encoder::a,
				constants::encoder::b,
				constants::encoder::sw
			);

			// Axis
			uint32_t _axisTickTime = 0;

			Axis _leverLeft = Axis(
				constants::axis::leverLeft::unit,
				constants::axis::leverLeft::channel,
				&_settings.axis.leverLeft
			);

			Axis _leverRight = Axis(
				constants::axis::leverRight::unit,
				constants::axis::leverRight::channel,
				&_settings.axis.leverRight
			);

			Axis _joystickHorizontal = Axis(
				constants::axis::joystickHorizontal::unit,
				constants::axis::joystickHorizontal::channel,
				&_settings.axis.joystickHorizontal
			);

			Axis _joystickVertical = Axis(
				constants::axis::joystickVertical::unit,
				constants::axis::joystickVertical::channel,
				&_settings.axis.joystickVertical
			);

			Axis _ring = Axis(
				constants::axis::ring::unit,
				constants::axis::ring::channel,
				&_settings.axis.ring
			);

			Battery _battery = Battery(
				constants::battery::unit,
				constants::battery::channel,

				constants::battery::voltageMin,
				constants::battery::voltageMax,

				constants::battery::voltageDividerR1,
				constants::battery::voltageDividerR2
			);

			// -------------------------------- UI --------------------------------

			Application _application;
			Layout _pageLayout;

			Menu* _menu = nullptr;
			DebugOverlay* _debugOverlay = nullptr;

			const Route* _route = nullptr;

			// -------------------------------- Aircraft data -------------------------------

			// Kronshtadt airfield in Saint-Petersburg for UI testing
			// ПРИВЕТУЛИ ФЕДИНОЙ ДАМЕ СЕРДЦА
			GeocentricCoordinates _geocentricCoordinates = GeocentricCoordinates(
				yoba::toRadians(60.01449883137194),
				yoba::toRadians(29.702554658332105),
				1000
			);

			Vector3F _cartesianCoordinates {};

			float _altimeterPressure = 1013;

			// -------------------------------- Flight path vector -------------------------------

			constexpr static const uint32_t _flightPathVectorInterval = 2'000'000;

			Vector3F _flightPathVectorCartesianCoordinates {};
			uint32_t _flightPathVectorTime = 0;
			Vector3F _flightPathVector {};
			Vector2F _flightPathAngles {};

			// -------------------------------- Timings --------------------------------

			uint32_t _tickDeltaTime = 0;
			uint32_t _interpolationTickTime = 0;
			uint32_t _aircraftPacketTime = 0;

			LowPassInterpolator _airspeedInterpolator;
			LowPassInterpolator _airspeedTrendInterpolator;

			LowPassInterpolator _altitudeInterpolator;
			LowPassInterpolator _altitudeTrendInterpolator;

			LowPassInterpolator _verticalSpeedInterpolator;

			LowPassInterpolator _pitchInterpolator;
			LowPassInterpolator _rollInterpolator;
			LowPassInterpolator _yawInterpolator;
			LowPassInterpolator _slipAndSkidInterpolator;

			// -------------------------------- Other shit --------------------------------

			static void SPIBusSetup();
			void ADCUnitsSetup();

			void axisTick();

			void encoderTick();

			void NVSSetup();

			void interpolationTick();

	};
}
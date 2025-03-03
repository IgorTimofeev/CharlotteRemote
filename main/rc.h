#pragma once


#include "src/main.h"
#include "src/ui.h"
#include "src/hardware/displays/ILI9341Display.h"
#include "src/hardware/touchPanels/FT6336UTouchPanel.h"

#include "settings.h"
#include "constants.h"
#include "interpolator.h"

#include "ui/theme.h"
#include "ui/navigation/tabBar.h"
#include "ui/debugOverlay.h"

#include "hardware/transceiver/packet.h"
#include "hardware/transceiver/WiFiTransceiver.h"
#include "hardware/speaker.h"
#include "hardware/axis.h"
#include "hardware/battery.h"
#include "hardware/encoder.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::hardware;
	using namespace yoba::ui;

	enum class AltimeterMode : uint8_t {
		QNH,
		QNE
	};

	class RC {
		public:
			static RC& getInstance();

			[[noreturn]] void run();

			Application& getApplication();

			Interpolator& getAirspeedInterpolator();
			Interpolator& getAltitudeInterpolator();
			Interpolator& getPitchInterpolator();
			Interpolator& getRollInterpolator();
			Interpolator& getYawInterpolator();
			Interpolator& getAirspeedTrendInterpolator();
			Interpolator& getAltitudeTrendInterpolator();
			Interpolator& getVerticalSpeedInterpolator();

			uint16_t* getThrottles();

			uint16_t getAilerons() const;
			void setAilerons(uint16_t ailerons);

			uint16_t getElevator() const;
			void setElevator(uint16_t elevator);

			uint16_t getRudder() const;
			void setRudder(uint16_t rudder);

			uint16_t getAileronsTrim() const;
			void setAileronsTrim(uint16_t aileronsTrim);

			uint16_t getElevatorTrim() const;
			void setElevatorTrim(uint16_t elevatorTrim);

			uint16_t getRudderTrim() const;
			void setRudderTrim(uint16_t rudderTrim);

			uint16_t getFlaps() const;
			void setFlaps(uint16_t flaps);

			uint16_t getSpoilers() const;
			void setSpoilers(uint16_t spoilers);

			bool isStrobeLights() const;
			void setStrobeLights(bool strobeLights);

			float getAltimeterPressure() const;
			void setAltimeterPressure(float altimeterPressure);

			AltimeterMode getAltimeterMode() const;
			void setAltimeterMode(AltimeterMode altimeterMode);

			float getLatitude() const;

			void setLatitude(float latitude);

			float getLongitude() const;

			void setLongitude(float longitude);

			void updateDebugOverlayVisibility();
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
			WiFiTransceiver _transceiver;

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
			TabBar _tabBar;
			DebugOverlay _debugOverlay;

			// -------------------------------- Remote data --------------------------------

			uint16_t _throttles[2] = {
				0xFFFF,
				0xFFFF
			};

			uint16_t _ailerons = 0xFFFF / 2;
			uint16_t _elevator = 0xFFFF / 2;
			uint16_t _rudder = 0xFFFF / 2;

			uint16_t _aileronsTrim = 0xFFFF / 2;
			uint16_t _elevatorTrim = 0xFFFF / 2;
			uint16_t _rudderTrim = 0xFFFF / 2;

			uint16_t _flaps = 0;
			uint16_t _spoilers = 0;

			AltimeterMode _altimeterMode = AltimeterMode::QNH;

			bool _strobeLights = false;

			// -------------------------------- Aircraft data --------------------------------

			// Kronshtadt airfield in Saint-Petersburg for UI testing
			float _latitude = yoba::toRadians(60.01449883137194);
			float _longitude = yoba::toRadians(29.702554658332105);
			float _altimeterPressure = 1013;

			// -------------------------------- Timings --------------------------------

			uint32_t _tickDeltaTime = 0;
			uint32_t _interpolationTickTime = 0;
			uint32_t _aircraftPacketTime = 0;

			Interpolator _airspeedInterpolator;
			Interpolator _airspeedTrendInterpolator;

			Interpolator _altitudeInterpolator;
			Interpolator _altitudeTrendInterpolator;

			Interpolator _verticalSpeedInterpolator;

			Interpolator _pitchInterpolator;
			Interpolator _rollInterpolator;
			Interpolator _yawInterpolator;

			// -------------------------------- Other shit --------------------------------

			static void SPIBusSetup();
			void ADCUnitsSetup();

			void axisTick();

			void encoderTick();

			void NVSSetup();

			void interpolationTick();
	};
}
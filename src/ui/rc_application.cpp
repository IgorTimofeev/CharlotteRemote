#include <cstdint>
#include "rc_application.h"
#include "resources/fonts.h"
#include "ui/navigation/side_bar.h"
#include "settings.h"

using namespace yoba;

namespace pizdanc {
	RCApplication::RCApplication() :
		Application(
			settings::pinout::screen::touch::reset,
			settings::pinout::screen::touch::interrupt
		)
	{

	}



	void RCApplication::begin() {
		// Screen
		Application::begin(4, settings::screen::palette);

		getScreen().setDefaultFont(resources::fonts::pixy10);

		// Workspace
		getWorkspace().addChild(new SideBar());

//		// Joysticks
//		_pitchHall.begin();
//		_rollHall.begin();

		// Transceiver
		_transceiver.begin();
	}

	uint32_t _testDeadline = 0;

	void RCApplication::onTick() {
		Application::onTick();

		if (millis() > _testDeadline) {
//			setSpeed(_rollHall.readSmoothFloat() * 40.0f);
//			setAltitude(_pitchHall.readSmoothFloat() * 20.0f);
//
//			setRoll((_rollHall.readSmoothFloat() * 2.0f - 1.0f) * (float) HALF_PI);
//			setPitch((_pitchHall.readSmoothFloat() * 2.0f - 1.0f) * (float) HALF_PI);

			getWorkspace().invalidate();

			_testDeadline = millis() + 50;
		}

		_transceiver.tick(*this);
	}

	void RCApplication::onRender() {
//		_piano.renderStrip();

		Application::onRender();
	}

	RCApplication& RCApplication::getInstance() {
		static RCApplication instance;

		return instance;
	}

	// ------------------------- Data -------------------------

	float RCApplication::getPitch() const {
		return _pitch;
	}

	void RCApplication::setPitch(float value) {
		_pitch = value;
	}

	float RCApplication::getRoll() const {
		return _roll;
	}

	void RCApplication::setRoll(float value) {
		_roll = value;
	}

	float RCApplication::getYaw() const {
		return _yaw;
	}

	void RCApplication::setYaw(float value) {
		_yaw = value;
	}

	float RCApplication::getAltitude() const {
		return _altitude;
	}

	void RCApplication::setAltitude(float value) {
		_altitude = value;
	}

	float RCApplication::getSpeed() const {
		return _speed;
	}

	void RCApplication::setSpeed(float speed) {
		_speed = speed;
	}

	float RCApplication::getPressure() const {
		return _pressure;
	}

	void RCApplication::setPressure(float value) {
		_pressure = value;
	}

	bool RCApplication::isPressureHPA() const {
		return _pressureHPA;
	}

	void RCApplication::setPressureHPA(bool value) {
		_pressureHPA = value;
	}

	bool RCApplication::isPressureSTD() const {
		return _pressureSTD;
	}

	void RCApplication::setPressureSTD(bool value) {
		_pressureSTD = value;
	}
}
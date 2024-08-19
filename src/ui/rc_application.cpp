#include <cstdint>
#include "rc_application.h"
#include "resources/fonts.h"
#include "ui/navigation/side_bar.h"

using namespace yoba;

namespace ui {
	RCApplication::RCApplication() : Application(
		Size(TFT_HEIGHT, TFT_WIDTH),
		25,

		21,
		22,
		12,
		14
	) {

	}

	void RCApplication::begin() {
		Application::begin();

		// Screen
		getScreen().setDefaultFont(resources::fonts::pixy10);

		// Background
		_palette[0]  = getScreen().getBuffer().color24to16(0x000000);
		_palette[1]  = getScreen().getBuffer().color24to16(0x111111);
		_palette[2]  = getScreen().getBuffer().color24to16(0x222222);
		_palette[3]  = getScreen().getBuffer().color24to16(0x333333);
		_palette[4]  = getScreen().getBuffer().color24to16(0x444444);

		// Foreground
		_palette[5]  = getScreen().getBuffer().color24to16(0xFFFFFF);
		_palette[6]  = getScreen().getBuffer().color24to16(0xDDDDDD);
		_palette[7]  = getScreen().getBuffer().color24to16(0xBBBBBB);
		_palette[8]  = getScreen().getBuffer().color24to16(0x999999);
		_palette[9]  = getScreen().getBuffer().color24to16(0x777777);

		// Purple
		_palette[10] = getScreen().getBuffer().color24to16(0xec04ee);
		// Green
		_palette[11] = getScreen().getBuffer().color24to16(0x008100);
		// Yellow
		_palette[12] = getScreen().getBuffer().color24to16(0xffd200);
		// Ocean
		_palette[13] = getScreen().getBuffer().color24to16(0x00ffff);
		// Ground
		_palette[14] = getScreen().getBuffer().color24to16(0xab6d2a);
		// Sky
		_palette[15] = getScreen().getBuffer().color24to16(0x006ffc);

		getScreen().setPalette(_palette);

		// Workspace
		getWorkspace().addChild(new SideBar());

		// Gavno
		_pitchHall.begin();
		_rollHall.begin();
	}

	uint32_t deadline = 0;

	void RCApplication::onTick() {
		Application::onTick();

		if (millis() > deadline) {
			setSpeed(_rollHall.readSmoothFloat() * 10.0f);
			setAltitude(_pitchHall.readSmoothFloat() * 10.0f);

			setRoll(_rollHall.readSmoothFloat() * 2 - 1.0f);
			setPitch(_pitchHall.readSmoothFloat() * 2 - 1.0f);

			getWorkspace().invalidate();

			deadline = millis() + 50;
		}
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
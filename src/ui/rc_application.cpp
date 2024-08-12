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
		getScreen().setDefaultFont(resources::fonts::unscii8);

		// Workspace
		getWorkspace().addChild(new SideBar());
//		getWorkspace().addChild(new Rectangle(Color::water));

//		// Piano
//		_piano.begin();
	}

	uint32_t deadline = 0;

	void RCApplication::onTick() {
		Application::onTick();

		if (millis() > deadline) {
			setAltitude(getAltitude() + 0.1f);
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

	void RCApplication::setPitch(float pitch) {
		_pitch = pitch;
	}

	float RCApplication::getRoll() const {
		return _roll;
	}

	void RCApplication::setRoll(float roll) {
		_roll = roll;
	}

	float RCApplication::getYaw() const {
		return _yaw;
	}

	void RCApplication::setYaw(float yaw) {
		_yaw = yaw;
	}

	float RCApplication::getAltitude() const {
		return _altitude;
	}

	void RCApplication::setAltitude(float altitude) {
		_altitude = altitude;
	}

	float RCApplication::getSpeed() const {
		return _speed;
	}

	void RCApplication::setSpeed(float speed) {
		_speed = speed;
	}
}
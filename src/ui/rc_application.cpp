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

		_transceiver.begin();
		_onboardLED.begin();
	}

	void RCApplication::tick() {
//		uint32_t startTime = millis();

		Application::tick();

//			setSpeed(_rollHall.readSmoothFloat() * 40.0f);
//			setAltitude(_pitchHall.readSmoothFloat() * 20.0f);
//
//			setRoll((_rollHall.readSmoothFloat() * 2.0f - 1.0f) * (float) HALF_PI);
//			setPitch((_pitchHall.readSmoothFloat() * 2.0f - 1.0f) * (float) HALF_PI);

		_transceiver.tick(*this);
		_onboardLED.tick();

//		uint32_t tickCost = millis() - startTime;

//		// Svit slip u stenki.........
//		if (tickCost < settings::application::tickBudget)
//			delay(settings::application::tickBudget - tickCost);
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

	LocalData &RCApplication::getLocalData() {
		return _localData;
	}

	RemoteData &RCApplication::getRemoteData() {
		return _remoteData;
	}

	OnboardLED &RCApplication::getOnboardLed() {
		return _onboardLED;
	}

	Transceiver& RCApplication::getTransceiver() {
		return _transceiver;
	}

	float RemoteData::getTemperature() const {
		return _temperature;
	}

	void RemoteData::setTemperature(float temperature) {
		_temperature = temperature;
	}

	float RemoteData::getPressure() const {
		return _pressure;
	}

	void RemoteData::setPressure(float pressure) {
		_pressure = pressure;
	}

	float RemoteData::getAltitude() const {
		return _altitude;
	}

	void RemoteData::setAltitude(float altitude) {
		_altitude = altitude;
	}

	float RemoteData::getPitch() const {
		return _pitch;
	}

	void RemoteData::setPitch(float pitch) {
		_pitch = pitch;
	}

	float RemoteData::getRoll() const {
		return _roll;
	}

	void RemoteData::setRoll(float roll) {
		_roll = roll;
	}

	float RemoteData::getYaw() const {
		return _yaw;
	}

	void RemoteData::setYaw(float yaw) {
		_yaw = yaw;
	}

	float RemoteData::getSpeed() const {
		return _speed;
	}

	void RemoteData::setSpeed(float speed) {
		_speed = speed;
	}

	uint8_t MutualData::getThrottle() const {
		return _throttle;
	}

	void MutualData::setThrottle(uint8_t throttle) {
		_throttle = throttle;
	}

	uint8_t MutualData::getAilerons() const {
		return _ailerons;
	}

	void MutualData::setAilerons(uint8_t ailerons) {
		_ailerons = ailerons;
	}

	uint8_t MutualData::getRudder() const {
		return _rudder;
	}

	void MutualData::setRudder(uint8_t rudder) {
		_rudder = rudder;
	}

	uint8_t MutualData::getFlaps() const {
		return _flaps;
	}

	void MutualData::setFlaps(uint8_t flaps) {
		_flaps = flaps;
	}

	AltimeterMode MutualData::getAltimeterMode() const {
		return _altimeterMode;
	}

	void MutualData::setAltimeterMode(AltimeterMode altimeterMode) {
		_altimeterMode = altimeterMode;
	}

	float MutualData::getAltimeterPressure() const {
		return _altimeterPressure;
	}

	void MutualData::setAltimeterPressure(float altimeterPressure) {
		_altimeterPressure = altimeterPressure;
	}

	bool MutualData::getStrobeLights() const {
		return _strobeLights;
	}

	void MutualData::setStrobeLights(bool strobeLights) {
		_strobeLights = strobeLights;
	}
}
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
		_tickTime = millis();

		setTickInterval(settings::application::tickInterval);
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
		auto oldSpeed = _speedPID.getTargetValue();
		auto oldAltitude = _altitudePID.getTargetValue();

		_transceiver.tick(*this);
		_onboardLED.tick();

		auto testDeltaTime = (float) (millis() - _testTickTime);
		float testDelay = 1000;

		if (testDeltaTime > testDelay) {
			_speedPID.setTargetValue(_speedPID.getTargetValue() + (float) random(0, 30) / 10.0f);

			if (_speedPID.getTargetValue() > 100)
				_speedPID.setTargetValue(0);

			_altitudePID.setTargetValue(_altitudePID.getTargetValue() + (float) random(0, 30) / 10.0f);

			if (_altitudePID.getTargetValue() > 500)
				_altitudePID.setTargetValue(0);

			_rollPID.setTargetValue(_rollPID.getTargetValue() + (float) radians(3));

			if (_rollPID.getTargetValue() > 1000)
				_rollPID.setTargetValue(0);

			_testTickTime = millis();

			auto newSpeed = _speedPID.getTargetValue();
			auto newAltitude = _altitudePID.getTargetValue();

			auto deltaSpeed = newSpeed - oldSpeed;
			auto deltaAltitude = newAltitude - oldAltitude;

			auto trendValueFactor = _trendValueDeltaTime / testDeltaTime;

			_speedTrendPID.setTargetValue(deltaSpeed * trendValueFactor);
			_altitudeTrendPID.setTargetValue(deltaAltitude * trendValueFactor);
			_verticalSpeedPID.setTargetValue(deltaAltitude * 60000.0f / testDeltaTime);
		}

		auto deltaTime = (float) (millis() - _tickTime);

		if (deltaTime > settings::application::tickInterval) {
			_speedPID.tick(deltaTime);
			_altitudePID.tick(deltaTime);
			_pitchPID.tick(deltaTime);
			_rollPID.tick(deltaTime);
			_yawPID.tick(deltaTime);
			_speedTrendPID.tick(deltaTime);
			_altitudeTrendPID.tick(deltaTime);
			_verticalSpeedPID.tick(deltaTime);

			getWorkspace().invalidate();

			_tickTime = millis();
		}

		Application::tick();
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

	OnboardLED &RCApplication::getOnboardLED() {
		return _onboardLED;
	}

	Transceiver& RCApplication::getTransceiver() {
		return _transceiver;
	}

	PID &RCApplication::getSpeedPid() {
		return _speedPID;
	}

	PID &RCApplication::getAltitudePid() {
		return _altitudePID;
	}

	PID &RCApplication::getPitchPid() {
		return _pitchPID;
	}

	PID &RCApplication::getRollPid() {
		return _rollPID;
	}

	PID &RCApplication::getYawPid() {
		return _yawPID;
	}

	PID &RCApplication::getSpeedTrendPid() {
		return _speedTrendPID;
	}

	PID &RCApplication::getAltitudeTrendPid() {
		return _altitudeTrendPID;
	}

	PID &RCApplication::getVerticalSpeedPid() {
		return _verticalSpeedPID;
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
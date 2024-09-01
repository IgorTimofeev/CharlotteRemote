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
		_transceiver.tick(*this);
		_onboardLED.tick();

		if (millis() > _tickTime + settings::application::tickInterval) {
			auto tickDeltaTime = (float) (millis() - _tickTime);

			auto oldSpeed = _remoteData.getSpeed();
			auto oldAltitude = _remoteData.getAltitude();

			_testSpeed = _testSpeed + (float) random(0, 30) / 10.0f * tickDeltaTime / 1000.0f;

			if (_testSpeed > 150)
				_testSpeed = 0;

			_testAltitude = _testAltitude + (float) random(0, 100) / 10.0f * tickDeltaTime / 1000.0f;

			if (_testAltitude > 1000)
				_testAltitude = 0;

			_testRoll = _testRoll + (float) radians(5) * tickDeltaTime / 1000.0f;

			if (degrees(_testRoll) > 45)
				_testRoll = 0;

			getRemoteData().setSpeed(_testSpeed);
			getRemoteData().setAltitude(_testAltitude);
			getRemoteData().setRoll(_testRoll);

			auto newSpeed = _remoteData.getSpeed();
			auto newAltitude = _remoteData.getAltitude();

			auto deltaSpeed = newSpeed - oldSpeed;
			auto deltaAltitude = newAltitude - oldAltitude;

			auto trendValueFactor = _trendValueDeltaTime / tickDeltaTime;
			getLocalData().setSpeedTrend(deltaSpeed * trendValueFactor);
			getLocalData().setAltitudeTrend(deltaAltitude * trendValueFactor);
			getLocalData().setVerticalSpeed(deltaAltitude * 60000.0f / tickDeltaTime);

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

	float LocalData::getSpeedTrend() const {
		return _speedTrend;
	}

	void LocalData::setSpeedTrend(float speedTrend) {
		_speedTrend = speedTrend;
	}

	float LocalData::getAltitudeTrend() const {
		return _altitudeTrend;
	}

	void LocalData::setAltitudeTrend(float altitudeTrend) {
		_altitudeTrend = altitudeTrend;
	}

	float LocalData::getVerticalSpeed() const {
		return _verticalSpeed;
	}

	void LocalData::setVerticalSpeed(float verticalSpeed) {
		_verticalSpeed = verticalSpeed;
	}
}
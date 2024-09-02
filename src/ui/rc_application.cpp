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
		auto oldSpeed = _speedInterpolator.getTargetValue();
		auto oldAltitude = _altitudeInterpolator.getTargetValue();

		_transceiver.tick(*this);
		_onboardLED.tick();

		auto testDeltaTime = (float) (millis() - _testTickTime);
		float testDelay = 1000;

		if (testDeltaTime > testDelay) {
			_speedInterpolator.setTargetValue(_speedInterpolator.getTargetValue() + (float) random(20, 20) / 10.0f);

			if (_speedInterpolator.getTargetValue() > 10)
				_speedInterpolator.setTargetValue(0);

			_altitudeInterpolator.setTargetValue(_altitudeInterpolator.getTargetValue() + (float) random(0, 30) / 10.0f);

			if (_altitudeInterpolator.getTargetValue() > 500)
				_altitudeInterpolator.setTargetValue(0);

			_rollInterpolator.setTargetValue(_rollInterpolator.getTargetValue() + (float) radians(3));

			if (_rollInterpolator.getTargetValue() > 1000)
				_rollInterpolator.setTargetValue(0);

			_testTickTime = millis();

			auto newSpeed = _speedInterpolator.getTargetValue();
			auto newAltitude = _altitudeInterpolator.getTargetValue();

			auto deltaSpeed = newSpeed - oldSpeed;
			auto deltaAltitude = newAltitude - oldAltitude;

			auto trendValueFactor = _trendValueDeltaTime / testDeltaTime;

			_speedTrendInterpolator.setTargetValue(deltaSpeed * trendValueFactor);
			_altitudeTrendInterpolator.setTargetValue(deltaAltitude * trendValueFactor);
			_verticalSpeedInterpolator.setTargetValue(deltaAltitude * 60000.0f / testDeltaTime);
		}

		auto deltaTime = (float) (millis() - _tickTime);

		if (deltaTime > settings::application::tickInterval) {
			const float interpolationFactor = deltaTime / testDelay;

			_speedInterpolator.tick(interpolationFactor);
			_speedTrendInterpolator.tick(interpolationFactor);

			_altitudeInterpolator.tick(interpolationFactor);
			_altitudeTrendInterpolator.tick(interpolationFactor);

			_verticalSpeedInterpolator.tick(interpolationFactor);

			_pitchInterpolator.tick(interpolationFactor);
			_rollInterpolator.tick(interpolationFactor);
			_yawInterpolator.tick(interpolationFactor);

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

	Interpolator &RCApplication::getSpeedInterpolator() {
		return _speedInterpolator;
	}

	Interpolator &RCApplication::getAltitudeInterpolator() {
		return _altitudeInterpolator;
	}

	Interpolator &RCApplication::getPitchInterpolator() {
		return _pitchInterpolator;
	}

	Interpolator &RCApplication::getRollInterpolator() {
		return _rollInterpolator;
	}

	Interpolator &RCApplication::getYawInterpolator() {
		return _yawInterpolator;
	}

	Interpolator &RCApplication::getSpeedTrendInterpolator() {
		return _speedTrendInterpolator;
	}

	Interpolator &RCApplication::getAltitudeTrendInterpolator() {
		return _altitudeTrendInterpolator;
	}

	Interpolator &RCApplication::getVerticalSpeedInterpolator() {
		return _verticalSpeedInterpolator;
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
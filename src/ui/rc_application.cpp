#include <cstdint>
#include "rc_application.h"
#include "settings.h"
#include "ui/navigation/side_bar.h"

using namespace yoba;

namespace pizdanc {
	RCApplication::RCApplication() : Application(
		&screenBuffer,
		&touchDriver
	) {
		_tickTime = millis();
	}

	void RCApplication::begin() {
		Application::begin();

		// Font
		setDefaultFont(&Theme::font);

		// Palette

		// Background
		screenBuffer.setPaletteColor(0, Rgb888Color(0x000000));
		screenBuffer.setPaletteColor(1, Rgb888Color(0x111111));
		screenBuffer.setPaletteColor(2, Rgb888Color(0x222222));
		screenBuffer.setPaletteColor(3, Rgb888Color(0x333333));
		screenBuffer.setPaletteColor(4, Rgb888Color(0x444444));

		// Foreground
		screenBuffer.setPaletteColor(5, Rgb888Color(0xFFFFFF));
		screenBuffer.setPaletteColor(6, Rgb888Color(0xDDDDDD));
		screenBuffer.setPaletteColor(7, Rgb888Color(0x999999));
		screenBuffer.setPaletteColor(8, Rgb888Color(0x777777));

		// Red
		screenBuffer.setPaletteColor(9, Rgb888Color(0xff0000));
		// Purple
		screenBuffer.setPaletteColor(10, Rgb888Color(0xff00ff));
		// Green
		screenBuffer.setPaletteColor(11, Rgb888Color(0x008100));
		// Yellow
		screenBuffer.setPaletteColor(12, Rgb888Color(0xffd200));
		// Ocean
		screenBuffer.setPaletteColor(13, Rgb888Color(0x00ffff));
		// Ground
		screenBuffer.setPaletteColor(14, Rgb888Color(0x97b838));
		// Sky
		screenBuffer.setPaletteColor(15, Rgb888Color(0x317fcb));

		// Workspace
		addChild(&_sideBar);

//		// Joysticks
//		_pitchHall.begin();
//		_rollHall.begin();

//		_transceiver.begin();
		_onboardLED.begin();
	}

	void RCApplication::tick() {
		auto oldSpeed = _speedInterpolator.getTargetValue();
		auto oldAltitude = _altitudeInterpolator.getTargetValue();

//		_transceiver.tick(*this);
		_onboardLED.tick();

		auto testDeltaTime = (float) (millis() - _testTickTime);
		float testDelay = 1000;

		if (testDeltaTime > testDelay) {
			// Speed
			_speedInterpolator.setTargetValue(_speedInterpolator.getTargetValue() + (float) random(1, 20) / 10.0f * testDeltaTime / testDelay);

			if (_speedInterpolator.getTargetValue() > 40)
				_speedInterpolator.setTargetValue(0);

			// Altitude
			_altitudeInterpolator.setTargetValue(_altitudeInterpolator.getTargetValue() + (float) random(1, 30) / 10.0f * testDeltaTime / testDelay);

			if (_altitudeInterpolator.getTargetValue() > 40)
				_altitudeInterpolator.setTargetValue(0);

			// Roll
			_rollInterpolator.setTargetValue(_rollInterpolator.getTargetValue() + (float) radians(3));

			if (degrees(_rollInterpolator.getTargetValue()) > 40)
				_rollInterpolator.setTargetValue(0);

			// A/P
			getLocalData().setAutopilotAltitude(20);
			getLocalData().setAutopilotSpeed(35);

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

			invalidate();

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

	float MutualData::getAutopilotSpeed() const {
		return _autopilotSpeed;
	}

	void MutualData::setAutopilotSpeed(float autopilotSpeed) {
		_autopilotSpeed = autopilotSpeed;
	}

	float MutualData::getAutopilotAltitude() const {
		return _autopilotAltitude;
	}

	void MutualData::setAutopilotAltitude(float autopilotAltitude) {
		_autopilotAltitude = autopilotAltitude;
	}
}
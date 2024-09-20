#include <cstdint>
#include "rc_application.h"
#include "settings.h"
#include "ui/navigation/menu.h"

using namespace yoba;

namespace pizdanc {
	RCApplication::RCApplication() :
		Application(
			&_screenBuffer,
			&_touchDriver
		)
	{
		_tickTime = millis();
	}

	void RCApplication::begin() {
		Application::begin();

		// Font
		setDefaultFont(&Theme::font);

		// Palette
		{
			uint8_t index = 0;

			auto add = [&](uint8_t r, uint8_t g, uint8_t b) {
				_screenBuffer.setPaletteColor(index++, Rgb888Color(r, g, b));
			};

			// Background
			add(0x00, 0x00, 0x00);
			add(0x11, 0x11, 0x11);
			add(0x22, 0x22, 0x22);
			add(0x33, 0x33, 0x33);
			add(0x44, 0x44, 0x44);

			// Foreground
			add(0xFF, 0xFF, 0xFF);
			add(0xDD, 0xDD, 0xDD);
			add(0x99, 0x99, 0x99);
			add(0x77, 0x77, 0x77);

			// Red
			add(0xff, 0x00, 0x00);
			// Purple
			add(0xff, 0x00, 0xff);
			// Green
			add(0x00, 0xFF, 0x00);
			// Green speed
			add(0x00, 0x81, 0x00);
			// Yellow
			add(0xff, 0xd2, 0x00);
			// Ocean
			add(0x00, 0xff, 0xff);
			// Ground
			add(0x97, 0xb8, 0x38);
			// Sky
			add(0x31, 0x7f, 0xcb);
		}

		// Workspace
		addChild(&_menu);

//		// Joysticks
//		_pitchHall.begin();
//		_rollHall.begin();

		_transceiver.begin();
		_onboardLED.begin();
	}

	void RCApplication::tick() {
		const auto oldSpeed = _speedInterpolator.getTargetValue();
		const auto oldAltitude = _altitudeInterpolator.getTargetValue();

		_transceiver.tick(*this);
		_onboardLED.tick();

		const auto testDeltaTime = (float) (millis() - _testTickTime);
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
			_testTickTime = millis();

			const auto newSpeed = _speedInterpolator.getTargetValue();
			const auto newAltitude = _altitudeInterpolator.getTargetValue();

			const auto deltaSpeed = newSpeed - oldSpeed;
			const auto deltaAltitude = newAltitude - oldAltitude;

			// Shows where spd/alt should target in 10 sec
			const float trendValueDeltaTime = 10 * 1000;
			const auto trendValueFactor = trendValueDeltaTime / testDeltaTime;

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
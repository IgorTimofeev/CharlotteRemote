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

	void RCApplication::setup() {
		Application::setup();

		// Palette
		_screenBuffer.setPaletteColors(Theme::palette, 256);

		// Font
		setDefaultFont(&Theme::font);

		// UI
		_menu.setup();
		*this += &_menu;

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
			// Throttle
			_throttleInterpolator.setTargetValue(_throttleInterpolator.getTargetValue() + 5);

			if (_throttleInterpolator.getTargetValue() > 100)
				_throttleInterpolator.setTargetValue(0);

			// Speed
			_speedInterpolator.setTargetValue(_speedInterpolator.getTargetValue() + (float) random(1, 20) / 10.0f * testDeltaTime / testDelay);

			if (_speedInterpolator.getTargetValue() > 40)
				_speedInterpolator.setTargetValue(0);

			// Altitude
			_altitudeInterpolator.setTargetValue(_altitudeInterpolator.getTargetValue() + (float) random(1, 30) / 10.0f * testDeltaTime / testDelay);

			if (_altitudeInterpolator.getTargetValue() > 40)
				_altitudeInterpolator.setTargetValue(0);

			// Pitch
			_pitchInterpolator.setTargetValue(_pitchInterpolator.getTargetValue() + (float) radians(2));

			if (degrees(_pitchInterpolator.getTargetValue()) > 10)
				_pitchInterpolator.setTargetValue(radians(-10));

			// Roll
			_rollInterpolator.setTargetValue(_rollInterpolator.getTargetValue() + (float) radians(1));

			if (degrees(_rollInterpolator.getTargetValue()) > 30)
				_rollInterpolator.setTargetValue(radians(-30));

			// Yaw
			_yawInterpolator.setTargetValue(_yawInterpolator.getTargetValue() + (float) radians(10));

			if (degrees(_yawInterpolator.getTargetValue()) > 170)
				_yawInterpolator.setTargetValue(radians(-170));

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

			// Controls
			auto handleControlSurfaces = [&](Interpolator& interpolator, float opposite = -1) {
				interpolator.setTargetValue(interpolator.getTargetValue() + 127.0f / 255.0f);

				if (interpolator.getTargetValue() >= 1)
					interpolator.setTargetValue(opposite);
			};

			handleControlSurfaces(_aileronsInterpolator);
			handleControlSurfaces(_flapsInterpolator, 0);
			handleControlSurfaces(_rudderInterpolator);
			handleControlSurfaces(_elevatorInterpolator);

			// Trim
			auto handleTrim = [&](Interpolator& interpolator) {
				interpolator.setTargetValue(interpolator.getTargetValue() + 0.1f);

				if (interpolator.getTargetValue() >= 1)
					interpolator.setTargetValue(0);
			};

			handleTrim(_aileronsTrimInterpolator);
			handleTrim(_rudderTrimInterpolator);
			handleTrim(_elevatorTrimInterpolator);
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

			_aileronsInterpolator.tick(interpolationFactor);
			_rudderInterpolator.tick(interpolationFactor);
			_elevatorInterpolator.tick(interpolationFactor);
			_flapsInterpolator.tick(interpolationFactor);

			_aileronsTrimInterpolator.tick(interpolationFactor);
			_elevatorTrimInterpolator.tick(interpolationFactor);
			_rudderTrimInterpolator.tick(interpolationFactor);

			invalidate();

			_tickTime = millis();
		}

		_menu.tick();

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

	Interpolator& RCApplication::getThrottleInterpolator() {
		return _throttleInterpolator;
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

	Interpolator& RCApplication::getAileronsInterpolator() {
		return _aileronsInterpolator;
	}

	Interpolator& RCApplication::getFlapsInterpolator() {
		return _flapsInterpolator;
	}

	Interpolator& RCApplication::getRudderInterpolator() {
		return _rudderInterpolator;
	}

	Interpolator& RCApplication::getElevatorInterpolator() {
		return _elevatorInterpolator;
	}

	Interpolator& RCApplication::getAileronsTrimInterpolator() {
		return _aileronsTrimInterpolator;
	}

	Interpolator& RCApplication::getElevatorTrimInterpolator() {
		return _elevatorTrimInterpolator;
	}

	Interpolator& RCApplication::getRudderTrimInterpolator() {
		return _rudderTrimInterpolator;
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

	float MutualData::getAutopilotHeading() const {
		return _autopilotHeading;
	}

	void MutualData::setAutopilotHeading(float autopilotHeading) {
		_autopilotHeading = autopilotHeading;
	}
}
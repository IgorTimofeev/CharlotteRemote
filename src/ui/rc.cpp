#include <cstdint>
#include "rc.h"
#include "settings.h"
#include "ui/navigation/menu.h"

namespace pizdanc {
	using namespace yoba;

	RC& RC::getInstance() {
		static RC instance;

		return instance;
	}

	void RC::setup() {
		// -------------------------------- Hardware --------------------------------

		// Application
		_application.setup(&_display, &_renderer, &_touchPanel);

		// Joysticks
//		_pitchHall.begin();
//		_rollHall.begin();

		// Other
		_transceiver.setup();
		_onboardLED.setup();

		// -------------------------------- UI --------------------------------

		// Theme
		Theme::setup(&_renderer);
		_application.setBackgroundColor(&Theme::bg1);

		// Menu
		_menu.setSelectedIndex(0);
		_application += &_menu;

		// Debug overlay
		_debugOverlay.setVisible(false);
		_application += &_debugOverlay;
	}

	void RC::tick() {
		auto time = system::getTime();

		simulateFlightData();

		_application.tick();

		_tickDeltaTime = system::getTime() - time;
	}

	void RC::simulateFlightData() {
		const auto oldSpeed = _speedInterpolator.getTargetValue();
		const auto oldAltitude = _altitudeInterpolator.getTargetValue();

		_transceiver.tick(*this);
		_onboardLED.tick();

		// Test
		const auto testDeltaTime = (float) (system::getTime() - _simulationTickTime2);
		float testDelay = 1000;

		if (testDeltaTime > testDelay) {
			// Throttle
			const auto handleFloat = [&](Interpolator& interpolator, float increment = 0.1f, float trigger = 0.05f) {
				if (std::abs(interpolator.getValue() - interpolator.getTargetValue()) > trigger)
					return;

				if (interpolator.getTargetValue() >= 1.0f) {
					interpolator.setTargetValue(0.0f);
				}
				else {
					interpolator.setTargetValue(std::min(interpolator.getTargetValue() + increment, 1.0f));
				}
			};

			handleFloat(_throttle1Interpolator, 0.2f, 0.01f);
			handleFloat(_throttle2Interpolator, 0.2f, 0.01f);

			// Speed
			_speedInterpolator.setTargetValue(_speedInterpolator.getTargetValue() + (float) random(1, 20) / 10.0f * testDeltaTime / testDelay);
//			_speedInterpolator.setTargetValue(_speedInterpolator.getTargetValue() + 2.0f);

			if (_speedInterpolator.getTargetValue() > 150)
				_speedInterpolator.setTargetValue(0);

			// Altitude
			_altitudeInterpolator.setTargetValue(_altitudeInterpolator.getTargetValue() + (float) random(1, 30) / 10.0f * testDeltaTime / testDelay);

			if (_altitudeInterpolator.getTargetValue() > 40)
				_altitudeInterpolator.setTargetValue(0);

			// Pitch
			_pitchInterpolator.setTargetValue(_pitchInterpolator.getTargetValue() + (float) toRadians(2));

			if (toDegrees(_pitchInterpolator.getTargetValue()) > 10)
				_pitchInterpolator.setTargetValue(toRadians(-10));

			// Roll
			_rollInterpolator.setTargetValue(_rollInterpolator.getTargetValue() + (float) toRadians(1));

			if (toDegrees(_rollInterpolator.getTargetValue()) > 30)
				_rollInterpolator.setTargetValue(toRadians(-30));

			// Yaw
			_yawInterpolator.setTargetValue(_yawInterpolator.getTargetValue() + (float) toRadians(10));

			if (toDegrees(_yawInterpolator.getTargetValue()) > 170)
				_yawInterpolator.setTargetValue(toRadians(-170));

			// A/P
			_simulationTickTime2 = system::getTime();

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
			handleFloat(_aileronsInterpolator);
			handleFloat(_flapsInterpolator);
			handleFloat(_rudderInterpolator);
			handleFloat(_elevatorInterpolator);
			handleFloat(_spoilersInterpolator);

			// Trim
			handleFloat(_aileronsTrimInterpolator);
			handleFloat(_rudderTrimInterpolator);
			handleFloat(_elevatorTrimInterpolator);
		}

		auto deltaTime = (float) (system::getTime() - _simulationTickTime1);

		if (deltaTime > settings::application::tickInterval) {
			const float interpolationFactor = deltaTime / testDelay;

			_throttle1Interpolator.tick(interpolationFactor);
			_throttle2Interpolator.tick(interpolationFactor);

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
			_spoilersInterpolator.tick(interpolationFactor);

			_aileronsTrimInterpolator.tick(interpolationFactor);
			_elevatorTrimInterpolator.tick(interpolationFactor);
			_rudderTrimInterpolator.tick(interpolationFactor);

			_application.invalidate();

			_simulationTickTime1 = system::getTime();
		}
	}

	// ------------------------- Data -------------------------

	LocalData &RC::getLocalData() {
		return _localData;
	}

	RemoteData &RC::getRemoteData() {
		return _remoteData;
	}

	OnboardLED &RC::getOnboardLED() {
		return _onboardLED;
	}

	Transceiver& RC::getTransceiver() {
		return _transceiver;
	}

	Interpolator& RC::getThrottle1Interpolator() {
		return _throttle1Interpolator;
	}

	Interpolator& RC::getThrottle2Interpolator() {
		return _throttle2Interpolator;
	}

	Interpolator &RC::getSpeedInterpolator() {
		return _speedInterpolator;
	}

	Interpolator &RC::getAltitudeInterpolator() {
		return _altitudeInterpolator;
	}

	Interpolator &RC::getPitchInterpolator() {
		return _pitchInterpolator;
	}

	Interpolator &RC::getRollInterpolator() {
		return _rollInterpolator;
	}

	Interpolator &RC::getYawInterpolator() {
		return _yawInterpolator;
	}

	Interpolator &RC::getSpeedTrendInterpolator() {
		return _speedTrendInterpolator;
	}

	Interpolator &RC::getAltitudeTrendInterpolator() {
		return _altitudeTrendInterpolator;
	}

	Interpolator &RC::getVerticalSpeedInterpolator() {
		return _verticalSpeedInterpolator;
	}

	Interpolator& RC::getAileronsInterpolator() {
		return _aileronsInterpolator;
	}

	Interpolator& RC::getFlapsInterpolator() {
		return _flapsInterpolator;
	}

	Interpolator& RC::getSpoilersInterpolator() {
		return _spoilersInterpolator;
	}

	Interpolator& RC::getRudderInterpolator() {
		return _rudderInterpolator;
	}

	Interpolator& RC::getElevatorInterpolator() {
		return _elevatorInterpolator;
	}

	Interpolator& RC::getAileronsTrimInterpolator() {
		return _aileronsTrimInterpolator;
	}

	Interpolator& RC::getElevatorTrimInterpolator() {
		return _elevatorTrimInterpolator;
	}

	Interpolator& RC::getRudderTrimInterpolator() {
		return _rudderTrimInterpolator;
	}

	Application& RC::getApplication() {
		return _application;
	}

	Element& RC::getDebugOverlay() {
		return _debugOverlay;
	}

	uint32_t RC::getTickDeltaTime() const {
		return _tickDeltaTime;
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
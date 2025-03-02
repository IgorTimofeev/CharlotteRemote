#include "data.h"

namespace pizda {
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

	float RemoteData::getLatitude() const {
		return _latitude;
	}

	void RemoteData::setLatitude(float latitude) {
		_latitude = latitude;
	}

	float RemoteData::getLongitude() const {
		return _longitude;
	}

	void RemoteData::setLongitude(float longitude) {
		_longitude = longitude;
	}

	uint8_t LocalData::getThrottle() const {
		return _throttle;
	}

	void LocalData::setThrottle(uint8_t throttle) {
		_throttle = throttle;
	}

	uint8_t LocalData::getAilerons() const {
		return _ailerons;
	}

	void LocalData::setAilerons(uint8_t ailerons) {
		_ailerons = ailerons;
	}

	uint8_t LocalData::getRudder() const {
		return _rudder;
	}

	void LocalData::setRudder(uint8_t rudder) {
		_rudder = rudder;
	}

	uint8_t LocalData::getFlaps() const {
		return _flaps;
	}

	void LocalData::setFlaps(uint8_t flaps) {
		_flaps = flaps;
	}

	AltimeterMode LocalData::getAltimeterMode() const {
		return _altimeterMode;
	}

	void LocalData::setAltimeterMode(AltimeterMode value) {
		_altimeterMode = value;
	}

	float LocalData::getAltimeterPressure() const {
		return _altimeterPressure;
	}

	void LocalData::setAltimeterPressure(float value) {
		_altimeterPressure = value;
	}

	bool LocalData::getStrobeLights() const {
		return _strobeLights;
	}

	void LocalData::setStrobeLights(bool strobeLights) {
		_strobeLights = strobeLights;
	}

	// -------------------------------- ComputedData --------------------------------

	SinAndCos& ComputedData::getLatitudeSinAndCos() {
		return _latitudeSinAndCos;
	}

	SinAndCos& ComputedData::getLongitudeSinAndCos() {
		return _longitudeSinAndCos;
	}

	SinAndCos& ComputedData::getRollSinAndCos() {
		return _rollSinAndCos;
	}

	SinAndCos& ComputedData::getPitchSinAndCos() {
		return _pitchSinAndCos;
	}

	SinAndCos& ComputedData::getYawSinAndCos() {
		return _yawSinAndCos;
	}
}
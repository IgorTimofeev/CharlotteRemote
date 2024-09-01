#pragma once

#include "cstdint"

class PID {
	public:
		PID(float value, float p, float i, float d) : _value(value), _p(p), _i(i), _d(d) {

		}

		void tick(float newValue, float deltaTime) {
			float error = newValue - _value;

			_integral += error * deltaTime;

			if (!isnan(_oldError) && deltaTime > 0) {
				_derivative = (error - _oldError) / deltaTime;
			}

			_oldError = error;

			_value = _p * error + _i * _integral + _d * _derivative;
		}

		float getValue() const {
			return _value;
		}

		void setValue(float value) {
			_value = value;
		}

	private:
		float _value = 0;
		float _p = 0;
		float _i = 0;
		float _d = 0;

		float _integral = 0;
		float _derivative = 0;
		float _oldError = NAN;
};
#pragma once

#include "cstdint"

class PID {
	public:
		PID(float p, float i, float d) : _p(p), _i(i), _d(d) {

		}

		void tick(float deltaTime) {
			if (isnan(_value)) {
				_value = _targetValue;
				_oldError = 0;
				return;
			}

			auto error = _targetValue - _value;

			_integral += error * deltaTime;

			if (deltaTime > 0)
				_derivative = (error - _oldError) / deltaTime;

			_value = _p * error + _i * _integral + _d * _derivative;

			_oldError = error;
		}

//		void tick(float deltaTime) {
//			if (isnan(_value)) {
//				_value = _targetValue;
//				return;
//			}
//
//			_value = _value + (_targetValue - _value) * deltaTime / 1000.0f;
//		}

		float getValue() const {
			return _value;
		}

		void setValue(float value) {
			_value = value;
		}

		float getTargetValue() const {
			return _targetValue;
		}

		void setTargetValue(float targetValue) {
			_targetValue = targetValue;
		}

	private:
		float _p = 0;
		float _i = 0;
		float _d = 0;
		float _targetValue = 0;

		float _integral = 0;
		float _derivative = 0;
		float _oldError = NAN;
		float _value = NAN;
};
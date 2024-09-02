#pragma once

#include "cstdint"

class Interpolator {
	public:
		void tick(float deltaTime, float interval) {
			if (isnan(_value)) {
				_value = _targetValue;
				return;
			}

			_value = _value + (_targetValue - _value) * deltaTime / 1000.0f;
		}

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
		float _targetValue = 0;
		float _value = NAN;
};
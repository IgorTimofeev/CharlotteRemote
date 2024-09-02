#pragma once

#include "cstdint"

class Interpolator {
	public:
		void tick(float factor) {
			if (isnan(_value)) {
				_value = _targetValue;
				return;
			}

			_value = _value + (_targetValue - _value) * factor;
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
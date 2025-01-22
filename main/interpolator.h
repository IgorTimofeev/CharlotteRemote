#pragma once

#include <cstdint>
#include <cmath>

class Interpolator {
	public:
		void tick(float factor) {
			_value = std::isnan(_value) ? _targetValue : _value + (_targetValue - _value) * factor;
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
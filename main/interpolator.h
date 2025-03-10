#pragma once

#include <cstdint>
#include <cmath>

class Interpolator {
	public:
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

	protected:
		float _targetValue = 0;
		float _value = 0;
};

class LowPassInterpolator : public Interpolator {
	public:
		void tick(float factor) {
			_value = _value * (1 - factor) + _targetValue * factor;
		}
};
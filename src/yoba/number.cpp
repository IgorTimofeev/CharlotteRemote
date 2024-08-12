#include "number.h"

namespace yoba {
	float Number::clampFloat(float value, float minimum, float maximum) {
		if (value < minimum)
			value = minimum;
		else if (value > maximum)
			value = maximum;

		return value;
	}

	double Number::clampDouble(double value, double minimum, double maximum) {
		if (value < minimum)
			value = minimum;
		else if (value > maximum)
			value = maximum;

		return value;
	}

	uint8_t Number::clampUint8(uint8_t value, uint8_t minimum, uint8_t maximum) {
		if (value < minimum)
			value = minimum;
		else if (value > maximum)
			value = maximum;

		return value;
	}

	int16_t Number::clampUint16(int16_t value, int16_t minimum, int16_t maximum) {
		if (value < minimum)
			value = minimum;
		else if (value > maximum)
			value = maximum;

		return value;
	}

	int32_t Number::clampInt32(int32_t value, int32_t minimum, int32_t maximum) {
		if (value < minimum)
			value = minimum;
		else if (value > maximum)
			value = maximum;

		return value;
	}
}

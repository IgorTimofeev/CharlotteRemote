#pragma once

#include <cstdint>
#include <cmath>

class LowPassFilter {
	public:
		static void apply(float& value, float targetValue, float factor) {
			value = value * (1.f - factor) + targetValue * factor;
		}
};
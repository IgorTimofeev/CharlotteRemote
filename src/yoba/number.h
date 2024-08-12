#pragma once

#include <cstdint>

namespace yoba {
	class Number {
		public:
			static float clampFloat(float value, float minimum = 0, float maximum = 1);

			static int16_t clampUint16(int16_t value, int16_t minimum, int16_t maximum);

			static int32_t clampInt32(int32_t value, int32_t minimum, int32_t maximum);

			static uint8_t clampUint8(uint8_t value, uint8_t minimum = 0, uint8_t maximum = 255);

			static double clampDouble(double value, double minimum = 0, double maximum = 1);
	};
}
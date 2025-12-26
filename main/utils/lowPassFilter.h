#pragma once

namespace pizda {
	class LowPassFilter {
		public:
			static void apply(float& value, float targetValue, float factor) {
				// Just in case
				factor = std::clamp(factor, 0.f, 1.f);
				
				value = value * (1.f - factor) + targetValue * factor;
			}
	};
}
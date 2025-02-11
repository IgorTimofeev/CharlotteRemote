#pragma once

#include <cstdint>
#include <cmath>

namespace pizdanc {
	class SinAndCos {
		public:
			explicit SinAndCos(float angle) {
				fromAngle(angle);
			}

			float getSin() const {
				return _sin;
			}

			void setSin(float sin) {
				_sin = sin;
			}

			float getCos() const {
				return _cos;
			}

			void setCos(float cos) {
				_cos = cos;
			}

			void fromAngle(float angle) {
				_sin = std::sinf(angle);
				_cos = std::cosf(angle);
			}

		private:
			float _sin = 0;
			float _cos = 0;
	};
}
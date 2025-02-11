#pragma once

#include <cstdint>
#include <cmath>

namespace pizda {
	class SinAndCos {
		public:
			explicit SinAndCos() : _sin(0), _cos(0) {

			}

			explicit SinAndCos(float angle) : SinAndCos() {
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
			float _sin;
			float _cos;
	};
}
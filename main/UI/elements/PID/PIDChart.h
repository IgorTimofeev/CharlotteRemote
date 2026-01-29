#pragma once

#include <YOBA/UI.h>
#include "types/generic.h"

namespace pizda {
	using namespace YOBA;

	class PIDChart : public Control {
		public:
			void setPIDCoefficients(const PIDCoefficients& coefficients);
			void setStep(const uint64_t interval);
			void setDeltaTime(const float deltaTime);

		protected:
			void onEvent(Event* event) override;
			void onRender(Renderer* renderer, const Bounds& bounds) override;

		private:
			PIDCoefficients _PIDCoefficients {};
			uint8_t _setpoint = 50;

			float _deltaTime = 1;
			uint64_t _step = 5'000'000;

			void updateSetpointFromPointerEvent(const int32_t pointerY);
	};
}
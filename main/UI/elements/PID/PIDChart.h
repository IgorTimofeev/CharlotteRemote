#pragma once

#include <YOBA/UI.h>
#include "types/generic.h"

namespace pizda {
	using namespace YOBA;

	class PIDChart : public Control, public BackgroundColorElement {
		public:
			PIDChart();
			const PIDCoefficients& getCoefficients() const;
			void setCoefficients(const PIDCoefficients& coefficients);
			void setStepCount(const uint16_t value);
			void setDeltaTime(const float deltaTime);

			float getValueMax() const;
			void setValueMax(const float value);
		protected:
			void onEvent(Event* event) override;
			void onRender(Renderer* renderer, const Bounds& bounds) override;

		private:
			PIDCoefficients _coefficients {};
			float _valueFactor = 0.5f;

			float _valueMax = 100;

			float _deltaTime = 1;
			uint16_t _stepCount = 20;

			void updateValueFactorFromPointerEvent(const int32_t pointerY);
	};
}
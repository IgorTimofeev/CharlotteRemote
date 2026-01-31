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
			void setStepsQuantity(const uint16_t stepsQuantity);
			void setDeltaTime(const float deltaTime);

		protected:
			void onEvent(Event* event) override;
			void onRender(Renderer* renderer, const Bounds& bounds) override;

		private:
			PIDCoefficients _coefficients {};
			uint8_t _setpoint = 50;

			float _deltaTime = 1;
			uint16_t _stepsQuantity = 20;

			void updateSetpointFromPointerEvent(const int32_t pointerY);
	};

	enum class PIDChartEditorFrequency : uint8_t {
		hz1,
		hz20,
		hz30,
		hz100,

		max = hz100
	};

	enum class PIDChartEditorSteps : uint8_t {
		steps5,
		steps10,
		steps20,

		max = steps20
	};

	class PIDChartEditor : public RelativeStackLayout {
		public:
			PIDChartEditor();

			const PIDCoefficients& getCoefficients() const {
				return _chart.getCoefficients();
			}

			void setCoefficients(const PIDCoefficients& coefficients) {
				_chart.setCoefficients(coefficients);
			}

		private:
			PIDChart _chart {};
			PIDChartEditorFrequency _frequency = PIDChartEditorFrequency::hz20;
			PIDChartEditorSteps _steps = PIDChartEditorSteps::steps10;

			RelativeStackLayout _buttonsRow {};
			Button _frequencyButton {};
			Button _stepsButton {};

			void addButton(Button& button);
			void updateFromFrequency();
			void updateFromSteps();
	};
}
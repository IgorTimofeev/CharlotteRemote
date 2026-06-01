#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "UI/navigation/page.h"
#include "UI/elements/titler.h"

#include "axisEditor.h"

namespace pizda {
	using namespace YOBA;

	class AxesSettingsPage : public ScrollViewPage {
		public:
			AxesSettingsPage();

		protected:
			void onTick() override;

		private:
			AxisEditor _leverLeftAxisEditor;
			Titler _leverLeftAxisEditorTitle = Titler("Throttle", &_leverLeftAxisEditor);

			AxisEditor _aileronsAxisEditor;
			Titler _aileronsAxisEditorTitle = Titler("Ailerons", &_aileronsAxisEditor);

			AxisEditor _elevatorAxisEditor;
			Titler _elevatorAxisEditorTitle = Titler("Elevator", &_elevatorAxisEditor);

			AxisEditor _rudderAxisEditor;
			Titler _rudderAxisEditorTitle = Titler("Rudder", &_rudderAxisEditor);

			AxisEditor _leverRightAxisEditor;
			Titler _leverRightAxisEditorTitle = Titler("Flaps", &_leverRightAxisEditor);

			Slider _lowPassFactorSlider;
			Titler _lowPassFactorSliderTitle = Titler("Low pass filter factor", &_lowPassFactorSlider);

			constexpr static uint8_t jitteringThresholdMaxValue = 100;
			Slider _jitteringThresholdSlider;
			Titler _jitteringThresholdSliderTitle = Titler("Jittering threshold", &_jitteringThresholdSlider);
	};
}
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
			Titler _leverLeftAxisEditorTitle = Titler(L"Throttle", &_leverLeftAxisEditor);

			AxisEditor _aileronsAxisEditor;
			Titler _aileronsAxisEditorTitle = Titler(L"Ailerons", &_aileronsAxisEditor);

			AxisEditor _elevatorAxisEditor;
			Titler _elevatorAxisEditorTitle = Titler(L"Elevator", &_elevatorAxisEditor);

			AxisEditor _rudderAxisEditor;
			Titler _rudderAxisEditorTitle = Titler(L"Rudder", &_rudderAxisEditor);

			AxisEditor _leverRightAxisEditor;
			Titler _leverRightAxisEditorTitle = Titler(L"Flaps", &_leverRightAxisEditor);

			Slider _lowPassFactorSlider;
			Titler _lowPassFactorSliderTitle = Titler(L"Low pass filter factor", &_lowPassFactorSlider);

			constexpr static uint8_t jitteringCutoffMaxValue = 100;
			Slider _jitteringCutoffFactorSlider;
			Titler _jitteringValueSliderTitle = Titler(L"Jittering cutoff value", &_jitteringCutoffFactorSlider);
	};
}
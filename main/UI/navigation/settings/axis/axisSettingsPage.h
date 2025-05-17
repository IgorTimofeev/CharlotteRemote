#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include "axisEditor.h"

#include <UI/navigation/page.h>
#include <UI/elements/titler.h>

namespace pizda {
	using namespace YOBA;

	class AxisSettingsPage : public ScrollViewPage {
		public:
			explicit AxisSettingsPage();

		protected:
			void onTick() override;

		private:
			AxisEditor _aileronsAxisEditor;
			Titler _aileronsAxisEditorTitle = Titler(L"Ailerons", &_aileronsAxisEditor);

			AxisEditor _elevatorAxisEditor;
			Titler _elevatorAxisEditorTitle = Titler(L"Elevator", &_elevatorAxisEditor);

			AxisEditor _rudderAxisEditor;
			Titler _rudderAxisEditorTitle = Titler(L"Rudder", &_rudderAxisEditor);

			AxisEditor _spoilersAxisEditor;
			Titler _spoilersAxisEditorTitle = Titler(L"Spoilers", &_spoilersAxisEditor);

			AxisEditor _flapsAxisEditor;
			Titler _flapsAxisEditorTitle = Titler(L"Flaps", &_flapsAxisEditor);

			Slider _lowPassFactorSlider;
			Titler _lowPassFactorSliderTitle = Titler(L"Low pass filter factor (0 - 100%)", &_lowPassFactorSlider);

			constexpr static uint8_t _jitteringCutoffMaxValue = 100;
			Slider _jitteringCutoffFactorSlider;
			Titler _jitteringValueSliderTitle = Titler(L"Jittering cutoff value (0 - 100)", &_jitteringCutoffFactorSlider);
	};
}
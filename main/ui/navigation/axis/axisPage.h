#pragma once

#include "../../components/yoba/src/main.h"
#include "../../components/yoba/src/ui.h"
#include "../page.h"
#include "../../titler.h"
#include "axisEditor.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class AxisPage : public ScrollViewPage {
		public:
			explicit AxisPage();

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

			constexpr static const uint8_t _jitteringCutoffMaxValue = 100;
			Slider _jitteringCutoffFactorSlider;
			Titler _jitteringValueSliderTitle = Titler(L"Jittering cutoff value (0 - 100)", &_jitteringCutoffFactorSlider);
	};
}
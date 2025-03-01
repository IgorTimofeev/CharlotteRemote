#pragma once

#include "src/main.h"
#include "src/ui.h"

#include "../page.h"
#include "axisEditor.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class ControlsPage : public ScrollViewPage {
		public:
			explicit ControlsPage();

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
			Titler _lowPassFactorSliderTitle = Titler(L"Response speed", &_lowPassFactorSlider);
	};
}
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
			~AxesSettingsPage() override;

		protected:
			void onTick() override;

		private:
			static int32_t _scrollPosition;

			AxisEditor _leverLeftAxisEditor;
			Titler _leverLeftAxisEditorTitle { "Throttle", &_leverLeftAxisEditor };

			AxisEditor _aileronsAxisEditor;
			Titler _aileronsAxisEditorTitle { "Ailerons", &_aileronsAxisEditor };

			AxisEditor _elevatorAxisEditor;
			Titler _elevatorAxisEditorTitle { "Elevator", &_elevatorAxisEditor };

			AxisEditor _rudderAxisEditor;
			Titler _rudderAxisEditorTitle { "Rudder", &_rudderAxisEditor };

			AxisEditor _leverRightAxisEditor;
			Titler _leverRightAxisEditorTitle { "Flaps", &_leverRightAxisEditor };

			Slider _EMAFilterFactorSlider;
			Titler _EMAFilterFactorSliderTitle { "EMA filter factor", &_EMAFilterFactorSlider };

			constexpr static uint8_t jitteringThresholdMaxValue = 100;
			Slider _jitteringThresholdSlider;
			Titler _jitteringThresholdSliderTitle { "Jittering threshold", &_jitteringThresholdSlider };
	};
}
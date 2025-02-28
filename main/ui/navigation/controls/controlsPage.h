#pragma once

#include "src/main.h"
#include "src/ui.h"

#include "../page.h"
#include "controlsView.h"
#include "axisEditor.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class ControlsPage : public ScrollViewPage {
		public:
			void setup();

		private:
			ControlsView _controls;

			AxisEditor _leverLeftEditor;
			AxisEditor _leverRightAxisEditor;
			AxisEditor _joystickHorizontalAxisEditor;
			AxisEditor _joystickVerticalAxisEditor;
			AxisEditor _ringAxisEditor;
	};
}
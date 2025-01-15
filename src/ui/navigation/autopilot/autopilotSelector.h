#pragma once

#include "Arduino.h"

#include "../../../../lib/YOBA/src/yoba.h"
#include "../../../../lib/YOBA/src/ui.h"

#include "../../elements/titler.h"
#include "../../elements/rows.h"

using namespace yoba;
using namespace yoba::ui;

namespace pizdanc {
	class AutopilotSelector : public Layout {
		public:
			explicit AutopilotSelector(uint8_t digitCount, const std::wstring_view& titleText, const std::wstring_view& buttonText);

			Rectangle background = Rectangle();
			StackLayout rows = StackLayout();
			Text title = Text();
			SevenSegment seven = SevenSegment();
			Knob knob = Knob();
			Button button = Button();

		private:
	};
}
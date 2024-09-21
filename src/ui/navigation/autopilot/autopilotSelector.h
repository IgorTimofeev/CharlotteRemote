#pragma once

#include "Arduino.h"

#include "../../../../lib/YOBA/src/ui/text.h"
#include "../../../../lib/YOBA/src/ui/sevenSegment.h"
#include "../../../../lib/YOBA/src/ui/rotaryKnob.h"
#include "../../../../lib/YOBA/src/ui/shapes/rectangle.h"
#include "../../../../lib/YOBA/src/ui/button.h"

#include "../../elements/titler.h"
#include "../../elements/rows.h"

using namespace yoba;

namespace pizdanc {
	class AutopilotSelector : public Layout {
		public:
			explicit AutopilotSelector(uint8_t digitCount, const wchar_t* titleText, const wchar_t *buttonText);

			Rectangle background = Rectangle();
			StackLayout rows = StackLayout();
			Text title = Text();
			SevenSegment seven = SevenSegment();
			RotaryKnob knob = RotaryKnob();
			Button button = Button();

		private:
	};
}
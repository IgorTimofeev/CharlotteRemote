#pragma once

#include "Arduino.h"

#include "../../../../lib/YOBA/src/ui/text.h"
#include "../../../../lib/YOBA/src/ui/sevenSegment.h"
#include "../../../../lib/YOBA/src/ui/knob.h"
#include "../../../../lib/YOBA/src/ui/rectangle.h"
#include "../../../../lib/YOBA/src/ui/button.h"

#include "../../elements/titler.h"
#include "../../elements/rows.h"

using namespace yoba;

namespace pizdanc {
	class AutopilotSelector : public Container {
		public:
			explicit AutopilotSelector(uint8_t digitCount, const std::wstring_view& titleText, const std::wstring_view& buttonText);

			Rectangle background = Rectangle();
			StackContainer rows = StackContainer();
			Text title = Text();
			SevenSegment seven = SevenSegment();
			Knob knob = Knob();
			Button button = Button();

		private:
	};
}
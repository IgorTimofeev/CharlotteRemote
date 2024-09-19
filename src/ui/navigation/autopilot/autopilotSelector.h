#pragma once

#include "Arduino.h"

#include "../../../../lib/YOBA/src/ui/text.h"
#include "../../../../lib/YOBA/src/ui/sevenSegment.h"
#include "../../../../lib/YOBA/src/ui/rotaryKnob.h"
#include "../../../../lib/YOBA/src/ui/shapes/rectangle.h"

#include "../../elements/titler.h"
#include "../../elements/rows.h"

using namespace yoba;

namespace pizdanc {
	class AutopilotSelector : public Layout {
		public:
			explicit AutopilotSelector(const wchar_t* titleText);

			Rectangle background = Rectangle();
			StackLayout rows = StackLayout();
			RotaryKnob rotaryKnob = RotaryKnob();
			SevenSegment sevenSegment = SevenSegment();
			Titler title = Titler();

		private:
			static void setIndicatorStyle(SevenSegment& indicator);

			static void setRotaryStyle(RotaryKnob& rotaryKnob);
	};
}
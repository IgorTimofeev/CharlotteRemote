#pragma once

#include "Arduino.h"

#include "../../../../lib/YOBA/src/ui/text.h"
#include "../../../../lib/YOBA/src/ui/slider.h"
#include "../../../../lib/YOBA/src/ui/sevenSegment.h"
#include "../../../../lib/YOBA/src/ui/knob.h"

#include "../page.h"
#include "../../elements/titler.h"
#include "../../elements/rows.h"
#include "autopilotSelector.h"

using namespace yoba;

namespace pizdanc {
	class RCApplication;

	class AutopilotPage : public Page {
		public:
			StackLayout columns = StackLayout();

			void begin() override;

		private:
			AutopilotSelector spd = AutopilotSelector(3, L"Speed", L"A/T");
			AutopilotSelector hdg = AutopilotSelector(3, L"Heading", L"HOLD");
			AutopilotSelector alt = AutopilotSelector(4, L"Altitude", L"FLC");
	};
}
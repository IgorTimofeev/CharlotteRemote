#pragma once

#include "Arduino.h"

#include "../../../../lib/YOBA/src/ui/text.h"
#include "../../../../lib/YOBA/src/ui/slider.h"
#include "../../../../lib/YOBA/src/ui/sevenSegment.h"
#include "../../../../lib/YOBA/src/ui/knob.h"

#include "../../theme.h"
#include "../page.h"
#include "../../elements/titler.h"
#include "../../elements/rows.h"
#include "autopilotSelector.h"

using namespace yoba;

namespace pizdanc {
	class AutopilotPage : public Page {
		public:
			void setup() override;

		private:
			StackLayout _columns {};

			AutopilotSelector _spd = AutopilotSelector(3, L"Speed", L"A/T");
			AutopilotSelector _hdg = AutopilotSelector(3, L"Heading", L"HOLD");
			AutopilotSelector _alt = AutopilotSelector(4, L"Altitude", L"FLC");
	};
}
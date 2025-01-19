#pragma once

#include "Arduino.h"

#include "../../../../lib/yoba/src/main.h"
#include "../../../../lib/yoba/src/ui.h"

#include "../page.h"
#include "autopilotSelector.h"

namespace pizdanc {
	using namespace yoba;

	class AutopilotPage : public Page {
		public:
			void setup() override;

		private:
			StackLayout _columns;

			AutopilotSelector _spd = AutopilotSelector(3, L"Speed", L"A/T");
			AutopilotSelector _hdg = AutopilotSelector(3, L"Heading", L"HOLD");
			AutopilotSelector _alt = AutopilotSelector(4, L"Altitude", L"FLC");
	};
}
#pragma once

#include "../../../../components/yoba/src/main.h"
#include "../../../../components/yoba/src/ui.h"

#include "../page.h"
#include "autopilotSelector.h"

namespace pizda {
	using namespace yoba;

	class AutopilotPage : public Page {
		public:
			void setup();

		private:
			StackLayout rows;

			AutopilotSelector _spd = AutopilotSelector(3, L"Speed", L"A/T");
			AutopilotSelector _hdg = AutopilotSelector(3, L"Heading", L"HDG");
			AutopilotSelector _alt = AutopilotSelector(4, L"Altitude", L"FLC");
	};
}
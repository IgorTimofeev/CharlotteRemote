#pragma once

#include "../../../../components/yoba/src/main.h"
#include "../../../../components/yoba/src/ui.h"

#include "../page.h"
#include "autopilotSelector.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class AutopilotPage : public Page {
		public:
			explicit AutopilotPage();

		private:
			StackLayout rows;

			AutopilotSelector _spd = AutopilotSelector(3, L"Speed", L"A/T");
			AutopilotSelector _hdg = AutopilotSelector(3, L"Heading", L"HDG");
			AutopilotSelector _alt = AutopilotSelector(4, L"Altitude", L"FLC");
	};
}
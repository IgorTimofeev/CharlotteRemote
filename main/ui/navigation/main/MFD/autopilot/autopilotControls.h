#pragma once

#include "../../../../../../components/yoba/src/main.h"
#include "../../../../../../components/yoba/src/ui.h"
#include "../../../../theme.h"
#include "../../../../titler.h"

#include "../rotaryControl.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class AutopilotControls : public Layout {
		public:
			explicit AutopilotControls();

			StackLayout row {};

			LeftButtonRotaryControl<3, 0, 400, 1, 10> speed = { L"A/T" };
			Titler speedTitle = Titler(L"Speed", &speed);

			LeftButtonRotaryControl<3, 0, 360, 1, 10> heading = { L"HLD" };
			Titler headingTitle = Titler(L"Heading", &heading);

			LeftButtonRotaryControl<4, 0, 15000, 100, 1000> altitude = { L"FLC" };
			Titler altitudeTitle = Titler(L"Altitude", &altitude);

		private:

	};
}
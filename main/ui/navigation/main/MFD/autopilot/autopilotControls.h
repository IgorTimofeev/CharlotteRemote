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

			LeftButtonRotaryControl speed = LeftButtonRotaryControl(L"A/T", 3);
			Titler speedTitle = Titler(L"SPD", &speed);

			LeftButtonRotaryControl heading = LeftButtonRotaryControl(L"HLD", 3);
			Titler headingTitle = Titler(L"HDG", &heading);

			LeftButtonRotaryControl altitude = LeftButtonRotaryControl(L"FLC", 4);
			Titler altitudeTitle = Titler(L"ALT", &altitude);

			LeftButtonRotaryControl pressure = LeftButtonRotaryControl(L"STD", 4);
			Titler pressureTitle = Titler(L"BARO", &pressure);
	};
}
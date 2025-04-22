#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include "../../../theme.h"

#include "../instrumentsView.h"
#include "../../../titler.h"

#include "../rotaryControl.h"

namespace pizda {
	using namespace YOBA;

	class AutopilotControls : public InstrumentsView {
		public:
			explicit AutopilotControls();

			RotaryControl<3, 0, 400, false, 1, 10> speed = { L"A/T" };
			Titler speedTitle = Titler(L"Speed", &speed);

			RotaryControl<3, 0, 360, true, 1, 10> heading = { L"HLD" };
			Titler headingTitle = Titler(L"Heading", &heading);

			RotaryControl<4, 0, 15000, false, 100, 1000> altitude = { L"FLC" };
			Titler altitudeTitle = Titler(L"Altitude", &altitude);

		private:

	};
}
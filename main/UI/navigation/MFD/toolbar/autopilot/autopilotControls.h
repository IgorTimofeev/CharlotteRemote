#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include <UI/theme.h>

#include <UI/navigation/MFD/toolbar/toolbar.h>
#include <UI/navigation/MFD/toolbar/instrumentIndicatorLayout.h>

#include <UI/navigation/MFD/rotaryControl.h>
#include <UI/navigation/MFD/korryButton.h>

namespace pizda {
	using namespace YOBA;

	class AutopilotControls : public Toolbar {
		public:
			explicit AutopilotControls();

			KorryButton FDButton { L"FD" };

			RotaryControl<3, 0, 400, false, 1, 10> speed = { L"A/T" };
			InstrumentIndicatorLayout speedLayout = { L"Speed", &speed, false };

			RotaryControl<3, 0, 360, true, 1, 10> heading = { L"HLD" };
			InstrumentIndicatorLayout headingLayout = { L"Heading", &heading, false };

			RotaryControl<4, 0, 15000, false, 100, 1000> altitude = { L"FLC" };
			InstrumentIndicatorLayout altitudeLayout = { L"Altitude", &altitude, false };

		private:

	};
}
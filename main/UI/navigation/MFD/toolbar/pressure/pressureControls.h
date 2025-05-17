#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include <UI/theme.h>

#include <UI/navigation/MFD/toolbar/toolbar.h>
#include <UI/navigation/MFD/toolbar/instrumentIndicatorLayout.h>

#include <UI/navigation/MFD/rotaryControl.h>

namespace pizda {
	using namespace YOBA;

	class PressureControls : public Toolbar {
		public:
			explicit PressureControls();

			RotaryControl<4, 800, 1090, false, 1, 10> pressure = { L"STD" };
			InstrumentIndicatorLayout pressureLayout = { L"Baro", &pressure, false };

			RotaryControl<4, 0, 15000, false, 10, 100> minimums = { L"ON" };
			InstrumentIndicatorLayout minimumsLayout = { L"Minimums", &minimums, false };
	};
}
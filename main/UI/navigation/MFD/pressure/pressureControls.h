#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include "../../../theme.h"

#include "../instrumentsView.h"
#include "../../../titler.h"

#include "../rotaryControl.h"

namespace pizda {
	using namespace YOBA;

	class PressureControls : public InstrumentsView {
		public:
			explicit PressureControls();

			RotaryControl<4, 800, 1090, false, 1, 10> pressure = { L"STD" };
			Titler pressureTitle = Titler(L"Baro", &pressure);

			RotaryControl<4, 0, 15000, false, 10, 100> minimums = { L"ON" };
			Titler minimumsTitle = Titler(L"Minimums", &minimums);
	};
}
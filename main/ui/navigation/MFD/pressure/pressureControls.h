#pragma once

#include "src/main.h"
#include "src/ui.h"
#include "../../../theme.h"

#include "../instrumentsView.h"
#include "../../../titler.h"

#include "../rotaryControl.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class PressureControls : public InstrumentsView {
		public:
			explicit PressureControls();

			RotaryControl<4, 800, 1090, false, 1, 10> pressure = { L"STD" };
			Titler pressureTitle = Titler(L"Baro", &pressure);

			RotaryControl<4, 0, 15000, false, 10, 100> minimums = { L"ON" };
			Titler minimumsTitle = Titler(L"Minimums", &minimums);
	};
}
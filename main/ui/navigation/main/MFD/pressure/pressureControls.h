#pragma once

#include "../../../../../../components/yoba/src/main.h"
#include "../../../../../../components/yoba/src/ui.h"
#include "../../../../theme.h"

#include "../instrumentsLayout.h"
#include "../../../../titler.h"

#include "../rotaryControl.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class PressureControls : public InstrumentsLayout {
		public:
			explicit PressureControls();

			RotaryControl<4, 800, 1090, false, 1, 10> pressure = { L"STD" };
			Titler pressureTitle = Titler(L"Baro", &pressure);

			RotaryControl<4, 0, 15000, false, 10, 100> minimums = { L"ON" };
			Titler minimumsTitle = Titler(L"Minimums", &minimums);
	};
}
#pragma once

#include "../../../../../../components/yoba/src/main.h"
#include "../../../../../../components/yoba/src/ui.h"
#include "../../../../theme.h"
#include "../../../../titler.h"

#include "../rotaryControl.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class PressureControls : public Layout {
		public:
			explicit PressureControls();

			StackLayout row {};

			LeftButtonRotaryControl pressure = LeftButtonRotaryControl(L"STD", 4);
			Titler pressureTitle = Titler(L"BARO", &pressure);

			SingleRotaryControl minimums = SingleRotaryControl(4);
			Titler minimumsTitle = Titler(L"Minimums", &minimums);
	};
}
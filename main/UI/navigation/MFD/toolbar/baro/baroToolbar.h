#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include "UI/theme.h"

#include "UI/navigation/MFD/toolbar/toolbar.h"
#include "UI/navigation/MFD/toolbar/toolbarSection.h"
#include "UI/navigation/MFD/toolbar/rotaryControl.h"

namespace pizda {
	using namespace YOBA;

	class BaroToolbar : public RowToolbar {
		public:
			explicit BaroToolbar();

//			RotaryControl<5, 9000, 11000, false, 1, 10> pressure { L"Baro" };
//			RotaryControl<4, 0, 15000, false, 1, 10> minimums { L"Minimums" };
	};
}
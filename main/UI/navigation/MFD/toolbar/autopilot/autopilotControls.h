#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include "UI/theme.h"

#include "UI/navigation/MFD/toolbar/toolbar.h"
#include "UI/navigation/MFD/toolbar/instrumentIndicatorLayout.h"
#include "UI/navigation/MFD/korryButton.h"
#include "UI/navigation/MFD/toolbar/rotaryControl.h"

namespace pizda {
	using namespace YOBA;

	class AutopilotControls : public Toolbar {
		public:
			explicit AutopilotControls();

			KorryButton FDButton { L"FD" };

			RotaryControl<3, 0, 400, false, 1, 10> speed { L"Speed", L"A/T" };
			RotaryControl<3, 0, 360, true, 1, 10> heading { L"Heading", L"HLD" };
			RotaryControl<4, 0, 15000, false, 100, 1000> altitude { L"Altitude", L"FLC" };

		private:

	};
}
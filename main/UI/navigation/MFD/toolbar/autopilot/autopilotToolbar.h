#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include "UI/theme.h"

#include "UI/navigation/MFD/toolbar/toolbar.h"
#include "UI/navigation/MFD/toolbar/toolbarSection.h"
#include "UI/navigation/MFD/toolbar/korryButton.h"
#include "UI/navigation/MFD/toolbar/rotaryControl.h"

#include "resources/images.h"

namespace pizda {
	using namespace YOBA;

	class AutopilotToolbar : public Toolbar {
		public:
			explicit AutopilotToolbar();
			
			KorryButton flightDirectorsButton { L"FD", &resources::images::menuIconMFDAutopilotFlightDirectors };

			RotaryControl<3, 0, 400, false, 1, 10> speed { L"Speed", L"A/T" };
			RotaryControl<3, 0, 360, true, 1, 10> heading { L"Heading", L"HLD" };
			RotaryControl<4, 0, 15000, false, 10, 100> altitude { L"Altitude", L"FLC" };
		
		private:

	};
}
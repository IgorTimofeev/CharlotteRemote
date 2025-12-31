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

	class AutopilotToolbar : public RowToolbar {
		public:
			explicit AutopilotToolbar();
			
			KorryButton flightDirectorButton { L"FD", &resources::images::menuIconMFDAutopilotFlightDirector };
			
			RotaryControl<3, 0, 350, false, 1, 1> speed { L"IAS", L"A/T" };
			RotaryControl<3, 0, 359, true, 1, 10> heading { L"HDG", L"HLD" };
			RotaryControl<4, 0, 35000, false, 1, 10> altitude { L"ALT", L"FLC" };
			
			KorryButton engageButton { L"A/P", &resources::images::menuIconMFDAutopilotEngage };
	};
}
#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include "UI/theme.h"

#include "UI/navigation/MFD/toolbar/toolbar.h"
#include "UI/navigation/MFD/toolbar/toolbarSection.h"
#include "UI/navigation/MFD/toolbar/toolbarButton.h"
#include "UI/navigation/MFD/toolbar/rotaryControl.h"

#include "resources/images.h"

namespace pizda {
	using namespace YOBA;

	class AutopilotToolbar : public RowToolbar {
		public:
			explicit AutopilotToolbar();
		
		protected:
			void onTick() override;
			
		private:
			ToolbarButton flightDirector { L"FD", &resources::images::menuIconMFDAutopilotFlightDirector };
			
			RotaryControl<3, 0, 350, false, 1, 10> speed { L"IAS" };
			RotaryControl<3, 1, 359, true, 1, 10> heading { L"HDG" };
			RotaryControl<4, 0, 35000, false, 10, 100> altitude { L"ALT" };
			
			ToolbarButton autopilot { L"A/P", &resources::images::menuIconMFDAutopilotEngage };
	};
}
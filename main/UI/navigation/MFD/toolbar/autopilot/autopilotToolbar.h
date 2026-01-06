#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include "UI/theme.h"

#include "UI/navigation/MFD/toolbar/toolbar.h"
#include "UI/navigation/MFD/toolbar/toolbarButton.h"
#include "UI/navigation/MFD/toolbar/autopilot/speedRotaryControl.h"
#include "UI/navigation/MFD/toolbar/autopilot/lateralRotaryControl.h"
#include "UI/navigation/MFD/toolbar/autopilot/verticalRotaryControl.h"

#include "resources/images.h"

namespace pizda {
	using namespace YOBA;
	
	class AutopilotToolbar : public RowToolbar {
		public:
			explicit AutopilotToolbar();
		
		private:
			ImageToolbarButton flightDirector { L"FD", &resources::images::menuIconMFDAutopilotFlightDirector };
			
			SpeedRotaryControl speed {};
			LateralRotaryControl lateral {};
			VerticalRotaryControl vertical {};
			
			ImageToolbarButton autopilot { L"A/P", &resources::images::menuIconMFDAutopilotEngage };
	};
}
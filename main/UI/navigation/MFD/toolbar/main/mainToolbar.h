#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "throttleSection.h"
#include "flightControlsIndicator.h"
#include "batteryIndicator.h"
#include "radioIndicator.h"

#include "UI/navigation/MFD/toolbar/toolbar.h"
#include "UI/navigation/MFD/toolbar/toolbarSection.h"

namespace pizda {
	using namespace YOBA;

	class MainToolbar : public Toolbar {
		public:
			MainToolbar();

		protected:
			void onTick() override;

		private:
			StackLayout leftRow {};
			StackLayout rightRow {};
			
			ThrottleSection _throttleSection {};

			FlightControlsIndicator _flightControlsIndicator {};
			ToolbarSection _flightControlsSection = { L"FCTL", &_flightControlsIndicator };

			BatteryIndicator _batteryIndicatorRC {};
			BatteryIndicator _batteryIndicatorAC {};
			StackLayout _batteryRows {};
			ToolbarSection _batterySection = { L"BAT", &_batteryRows };

			RadioIndicator _radio {};
			ToolbarSection _radioSection = { L"SIG", &_radio };
	};
}
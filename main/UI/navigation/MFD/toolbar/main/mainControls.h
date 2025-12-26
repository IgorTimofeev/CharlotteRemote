#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "throttleIndicatorLayout.h"
#include "flightControlsIndicator.h"
#include "batteryIndicator.h"
#include "radioIndicator.h"

#include <UI/navigation/MFD/toolbar/toolbar.h>
#include <UI/navigation/MFD/toolbar/instrumentIndicatorLayout.h>

namespace pizda {
	using namespace YOBA;

	class MainControls : public Toolbar {
		public:
			MainControls();

		protected:
			void onTick() override;

		private:
			ThrottleIndicatorLayout _throttleIndicatorLayout {};

			FlightControlsIndicator _flightControlsIndicator {};
			InstrumentIndicatorLayout _flightControlsLayout = { L"FCTL", &_flightControlsIndicator };

			BatteryIndicator _batteryIndicatorRC {};
			BatteryIndicator _batteryIndicatorAC {};
			StackLayout _batteryRows {};
			InstrumentIndicatorLayout _batteryLayout = { L"BAT", &_batteryRows };

			RadioIndicator _radio {};
			InstrumentIndicatorLayout _radioLayout = { L"SIG", &_radio };
	};
}
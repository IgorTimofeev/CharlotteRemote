#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "throttleIndicatorLayout.h"
#include "flapsAndSpoilersIndicator.h"
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

			StackLayout _controlsRows {};
			FlapsAndSpoilersIndicator _flapsAndSpoilersIndicator {};
			ImageView _landingGearImageView {};
			InstrumentIndicatorLayout _controlsLayout = { L"CTL", &_controlsRows, true };

			BatteryIndicator _batteryIndicatorController {};
			BatteryIndicator _batteryIndicatorAircraft {};
			StackLayout _batteryIndicatorRows {};
			InstrumentIndicatorLayout _batteryLayout = { L"BAT", &_batteryIndicatorRows, true };

			RadioIndicator _radio {};
			InstrumentIndicatorLayout _radioLayout = { L"SIG", &_radio, true };
	};
}
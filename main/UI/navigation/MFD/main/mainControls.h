#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "trimIndicator.h"
#include "throttleIndicatorLayout.h"
#include "flapsAndSpoilersIndicator.h"
#include "batteryIndicator.h"

#include "../instrumentsView.h"
#include "../instrumentIndicatorLayout.h"
#include "../../menu/openMenuButton.h"

namespace pizda {
	using namespace YOBA;

	class MainControls : public InstrumentsView {
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

			TrimIndicator _elevatorTrimIndicator {};
			InstrumentIndicatorLayout _elevatorTrimLayout = { L"TRM", &_elevatorTrimIndicator, true };

			BatteryIndicator _batteryIndicatorController {};
			BatteryIndicator _batteryIndicatorAircraft {};
			StackLayout _batteryIndicatorRows {};
			InstrumentIndicatorLayout _batteryLayout = { L"BAT", &_batteryIndicatorRows, true };
	};
}
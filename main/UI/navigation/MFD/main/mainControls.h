#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "trimIndicator.h"
#include "throttleIndicator.h"
#include "flapsAndSpoilersIndicator.h"
#include "batteryIndicator.h"

#include "../instrumentsView.h"
#include "../../menu/openMenuButton.h"
#include "../../../titler.h"

namespace pizda {
	using namespace YOBA;

	class MainControls : public InstrumentsView {
		public:
			MainControls();

			constexpr static const uint8_t panelSize = 48;

		protected:
			void onTick() override;

			void onEvent(Event* event) override;

		private:
			ThrottleIndicator _throttle1Indicator {};
			ThrottleIndicator _throttle2Indicator {};

			StackLayout _throttleRow {};
			Titler _throttleTitle = Titler(L"THR", &_throttleRow);

			StackLayout _controlsRows {};
			FlapsAndSpoilersIndicator _flapsAndSpoilersIndicator {};
			ImageView _landingGearImageView {};
			Titler _controlsTitle = Titler(L"CTL", &_controlsRows);

			TrimIndicator _elevatorTrimIndicator {};
			Titler _elevatorTrimTitle = Titler(L"TRM", &_elevatorTrimIndicator);

			BatteryIndicator _batteryIndicatorController {};
			BatteryIndicator _batteryIndicatorAircraft {};
			StackLayout _batteryIndicatorRows {};
			Titler _batteryIndicatorTitle = Titler(L"BAT", &_batteryIndicatorRows);
	};
}
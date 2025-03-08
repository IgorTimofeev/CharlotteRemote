#pragma once

#include "../../../../../components/yoba/src/main.h"
#include "../../../../../components/yoba/src/ui.h"

#include "trimIndicator.h"
#include "throttleIndicator.h"
#include "flapsAndSpoilersIndicator.h"
#include "batteryIndicator.h"
#include "../../../../resources/images/PFDLandingGearExtendedImage.h"
#include "../../../../resources/images/PFDLandingGearRetractedImage.h"
#include "../../menuButton.h"
#include "../../../titler.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class InstrumentsLayout : public Layout {
		public:
			InstrumentsLayout();

			constexpr static const uint8_t panelSize = 52;

		protected:
			void onTick() override;

			void onEvent(Event* event) override;

		private:
			StackLayout _row;

			Rectangle _titlesRect;

			MenuButton _menuButton;

			ThrottleIndicator _throttle1Indicator;
			ThrottleIndicator _throttle2Indicator;

			StackLayout _throttleRow;
			Titler _throttleTitle = Titler(L"THR", &_throttleRow);

			StackLayout _controlsRows;
			FlapsAndSpoilersIndicator _flapsAndSpoilersIndicator;
			ImageView _landingGearImageView;
			Titler _controlsTitle = Titler(L"CTL", &_controlsRows);

			static const PFDLandingGearExtendedImage _landingGearExtendedImage;
			static const PFDLandingGearRetractedImage _landingGearRetractedImage;

			TrimIndicator _elevatorTrimIndicator;
			Titler _elevatorTrimTitle = Titler(L"TRM", &_elevatorTrimIndicator);

			BatteryIndicator _batteryIndicatorController;
			BatteryIndicator _batteryIndicatorAircraft;
			StackLayout _batteryIndicatorRows;
			Titler _batteryIndicatorTitle = Titler(L"BAT", &_batteryIndicatorRows);
	};
}
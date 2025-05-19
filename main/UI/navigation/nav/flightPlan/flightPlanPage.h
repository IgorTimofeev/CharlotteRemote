#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include <UI/navigation/page.h>
#include <UI/theme.h>

#include "flightPlanItem.h"
#include "placeholderButtons.h"

namespace pizda {
	using namespace YOBA;

	class FlightPlanPage : public ScrollViewPage {
		public:
			explicit FlightPlanPage();
			~FlightPlanPage() override;

			static FlightPlanPage* getInstance();

			void updateFromNavigationData();

		private:
			static FlightPlanPage* _instance;

			StackLayout _itemsLayout {};

			Text _originTitle {};
			AirportFlightPlanItem _originItem { false };
			AirportPlaceholderButton _originPlaceholderItem { false };

			Text _legsTitle {};
			StackLayout _legsLayout {};
			LegPlaceholderButton _legPlaceholderButton {};

			Text _destinationTitle {};
			AirportFlightPlanItem _destinationItem { true };
			AirportPlaceholderButton _destinationPlaceholderItem { true };

			static void applyTitleStyle(Text& text);
	};
}
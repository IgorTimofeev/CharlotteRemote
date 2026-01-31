#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "UI/navigation/page.h"

#include "legPlaceholderButton.h"

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

			StackLayout _legsLayout {};
			LegPlaceholderButton _addLegButton {};

			static void applyTitleStyle(TextView& text);
	};
}
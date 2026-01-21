#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "UI/navigation/page.h"
#include "UI/elements/tabSelector.h"
#include "UI/elements/tabSelector.h"

#include "waypointItem.h"

namespace pizda {
	using namespace YOBA;

	class WaypointsPage : public ScrollViewPage {
		public:
			explicit WaypointsPage();
			~WaypointsPage() override;

			static WaypointsPage* getInstance();

			void updateFromNavigationData();

		private:
			static WaypointsPage* _instance;

			TabSelectorItem _runwayTypeItem {};
			TabSelectorItem _enrouteTypeItem {};
			TabSelector _typeSelector {};

			RelativeStackLayout _searchAndAddRow {};
			TextField _searchTextField {};
			Button _addButton {};

			StackLayout _itemsLayout {};
	};
}

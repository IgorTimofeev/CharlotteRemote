#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "UI/navigation/page.h"
#include "UI/elements/tabSelector.h"
#include "UI/elements/tabSelector.h"

#include "waypointItem.h"

namespace pizda {
	using namespace YOBA;

	class DatabasePage : public ScrollViewPage {
		public:
			explicit DatabasePage();
			~DatabasePage() override;

			static DatabasePage* getInstance();

			void updateFromNavigationData();

		private:
			static DatabasePage* _instance;

			TabSelectorItem _runwayTypeItem {};
			TabSelectorItem _enrouteTypeItem {};
			TabSelector _typeSelector {};

			RelativeStackLayout _searchAndAddRow {};
			TextField _searchTextField {};
			Button _addButton {};

			StackLayout _itemsLayout {};
	};
}

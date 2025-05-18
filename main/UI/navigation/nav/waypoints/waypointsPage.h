#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include <UI/navigation/page.h>

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

			TextField _searchTextField {};
			StackLayout _waypointsRows {};

			void deleteItems();
			void search();
	};
}
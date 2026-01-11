#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "UI/navigation/menu/pagesMenuView.h"
#include "UI/navigation/menu/pageMenuViewButton.h"
#include "UI/navigation/routes.h"
#include <resources/images.h>

namespace pizda {
	using namespace YOBA;

	class NavMenuView : public PagesMenuView {
		public:
			explicit NavMenuView();

		private:
			static const Route* _lastRoute;

			std::array<PageMenuViewButton, 2> _buttons {
				PageMenuViewButton(&resources::images::menuIconFlightPlan, L"Flight plan", &Routes::navFlightPlan),
				PageMenuViewButton(&resources::images::menuIconWaypointsImage, L"Database", &Routes::navWaypoints)
			};
	};
}
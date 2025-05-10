#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "../menu/pagesMenuView.h"
#include "../menu/pageMenuViewButton.h"
#include "../routes.h"
#include "../../../resources/images.h"

namespace pizda {
	using namespace YOBA;

	class NavMenuView : public PagesMenuView {
		public:
			explicit NavMenuView();

		private:
			static const Route* _lastRoute;

			std::array<PageMenuViewButton, 1> _buttons {
				PageMenuViewButton(&resources::Images::menuIconMFDFlightPlan, L"Flight plan", &Routes::navFlightPlan)
			};
	};
}
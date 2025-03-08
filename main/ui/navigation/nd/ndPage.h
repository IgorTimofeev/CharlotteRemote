#pragma once

#include "../page.h"
#include "../menuButton.h"

#include "nd.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class NDPage : public Page {
		public:
			NDPage();

		private:
			ND _nd;

			StackLayout _slidersLayout;
			Slider _projectionSlider;
			Slider _clipSlider;
			MenuButton _menuButton;
	};
}
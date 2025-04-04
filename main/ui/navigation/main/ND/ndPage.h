#pragma once

#include "../../components/yoba/src/main.h"
#include "../../components/yoba/src/ui.h"

#include "../../page.h"
#include "../../menuButton.h"

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
			Slider _FOVSlider;
			Slider _nearSlider;
			MenuButton _menuButton;
	};
}
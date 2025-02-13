#pragma once

#include "../page.h"
#include "nd.h"

namespace pizda {
	using namespace yoba;

	class NDPage : public Page {
		public:
			NDPage();

		private:
			ND _nd;

			StackLayout _slidersLayout;
			Slider _fovSlider;
			Slider _clipSlider;
	};
}
#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "../page.h"
#include "../../elements/titler.h"
#include "../../elements/switcher.h"

namespace pizda {
	using namespace YOBA;

	class PersonalizationPage : public ScrollViewPage {
		public:
			PersonalizationPage();

		private:
			Slider _FOVSlider {};
			Titler _FOVTitle = Titler(&_FOVSlider);

			constexpr static const uint8_t FOVMin = 30;
			constexpr static const uint8_t FOVMax = 180;

			void updateFOVTitle();
	};
}
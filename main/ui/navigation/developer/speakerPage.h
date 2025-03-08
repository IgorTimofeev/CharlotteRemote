#pragma once

#include "../../../components/yoba/src/main.h"
#include "../../../components/yoba/src/ui.h"

#include "../page.h"
#include "../../titler.h"
#include "../../switcher.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class SpeakerPage : public ScrollViewPage {
		public:
			SpeakerPage();

			Slider _frequencySlider;
			Titler _frequencySliderTitle = Titler(L"Frequency (1 - 12 KHz)", &_frequencySlider);

			Slider _durationSlider;
			Titler _durationSliderTitle = Titler(L"Duration (1 - 1000 ms)", &_durationSlider);

			Slider _countSlider;
			Titler _countSliderTitle = Titler(L"Count (1 - 5 times)", &_countSlider);
			Text _countText;

			Button _button;
	};
}
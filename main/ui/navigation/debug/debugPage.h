#pragma once

#include "src/main.h"
#include "src/ui.h"
#include "../page.h"

namespace pizdanc {
	using namespace yoba;
	using namespace yoba::ui;

	class DebugPage : public Page {
		public:
			DebugPage();

		private:
			ScrollView _scrollView;

			StackLayout _rows;

			Text _pageTitle;

			Slider _speakerFrequencySlider;
			Titler _speakerFrequencySliderTitle = Titler(L"Speaker frequency", &_speakerFrequencySlider);

			Slider _speakerDurationSlider;
			Titler _speakerDurationSliderTitle = Titler(L"Speaker duration", &_speakerDurationSlider);

			Slider _speakerCountSlider;
			Titler _speakerCountSliderTitle = Titler(L"Speaker count", &_speakerCountSlider);
			Text _speakerCountText;

			Button _speakerButton;

			Slider _textFontSizeSlider;
			Titler _textSliderTitle = Titler(L"Font size slider", &_textFontSizeSlider);

			Slider _textMarginSlider;
			Titler _textMarginSliderTitle = Titler(L"Margin slider", &_textMarginSlider);

			Text _text;

			ProgressBar _progressBar;
			Titler _progressBarTitle = Titler(L"Progress bar", &_progressBar);

			Switch _switch;
			Titler _switchTitle = Titler(L"Switch", &_switch);

			TextField _textField;
			Titler textFieldTitle = Titler(L"Text", &_textField);
	};
}
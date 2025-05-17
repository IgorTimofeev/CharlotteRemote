#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include <UI/navigation/page.h>
#include <UI/elements/titler.h>

namespace pizda {
	using namespace YOBA;

	class DeveloperSettingsPage : public ScrollViewPage {
		public:
			explicit DeveloperSettingsPage();

		private:
			Slider _speakerFrequencySlider;
			Titler _speakerFrequencySliderTitle = Titler(L"Frequency (1 - 12 KHz)", &_speakerFrequencySlider);

			Slider _speakerDurationSlider;
			Titler _speakerDurationSliderTitle = Titler(L"Duration (1 - 1000 ms)", &_speakerDurationSlider);

			Slider _speakerCountSlider;
			Titler _speakerCountSliderTitle = Titler(L"Count (1 - 5 times)", &_speakerCountSlider);
			Text _speakerCountText;

			Button _speakerButton;

			Slider _textFontSizeSlider;
			Titler _textSliderTitle = Titler(L"Font size slider", &_textFontSizeSlider);

			Slider _textMarginSlider;
			Titler _textMarginSliderTitle = Titler(L"Margin slider", &_textMarginSlider);

			Text _text;

			ProgressBar _progressBar;
			Titler _progressBarTitle = Titler(L"Progress bar", &_progressBar);

			Switch _debugOverlaySwitch;
			Titler _debugOverlaySwitchTitle = Titler(L"Switch", &_debugOverlaySwitch);

			TextField _textField;
			Titler textFieldTitle = Titler(L"Text", &_textField);

	};
}
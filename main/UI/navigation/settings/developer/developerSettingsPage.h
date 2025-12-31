#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include <UI/navigation/page.h>
#include <UI/elements/titler.h>

#include "hardware/speaker/sound.h"

namespace pizda {
	using namespace YOBA;

	class DeveloperSettingsPage : public ScrollViewPage {
		public:
			explicit DeveloperSettingsPage();

		private:
			Button _settingsNavigationResetButton {};

			// Speaker
			Text _speakerTitle {};

			Slider _speakerFrequencySlider {};
			Titler _speakerFrequencySliderTitle { L"Frequency (1 - 12 KHz)", &_speakerFrequencySlider };

			Slider _speakerDurationSlider {};
			Titler _speakerDurationSliderTitle { L"Duration (1 - 1000 ms)", &_speakerDurationSlider };

			Slider _speakerCountSlider {};
			Titler _speakerCountSliderTitle { L"Count (1 - 5 times)", &_speakerCountSlider };
			Text _speakerCountText {};

			std::array<Note, 8> _speakerNotes {};
			std::array<Delay, 8> _speakerDelays {};
			std::array<Playable*, 16> _speakerPlayables {};
			Sound _speakerSound {};
			Button _speakerButton {};

			// Text
			Slider _textFontSizeSlider {};
			Titler _textSliderTitle { L"Font size slider", &_textFontSizeSlider };

			Slider _textMarginSlider {};
			Titler _textMarginSliderTitle { L"Margin slider", &_textMarginSlider };

			Text _text {};

			ProgressBar _progressBar {};
			Titler _progressBarTitle { L"Progress bar", &_progressBar };

			TextField _textField {};
			Titler textFieldTitle { L"Text", &_textField };

	};
}
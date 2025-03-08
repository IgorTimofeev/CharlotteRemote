#pragma once

#include "../../../components/yoba/src/main.h"
#include "../../../components/yoba/src/ui.h"

#include "../page.h"
#include "../../titler.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class UITestPage : public ScrollViewPage {
		public:
			explicit UITestPage();

		private:
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
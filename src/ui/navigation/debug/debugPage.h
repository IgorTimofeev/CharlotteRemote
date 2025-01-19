#pragma once

#include "Arduino.h"

#include "../../../../lib/yoba/src/main.h"
#include "../../../../lib/yoba/src/ui.h"

#include "ui/navigation/page.h"

namespace pizdanc {
	using namespace yoba;
	using namespace yoba::ui;

	class DebugPage : public Page {
		public:
			DebugPage();

		private:
			StackLayout _rows = StackLayout();

			Text _pageTitle;

			Text _wrappedText;

			Slider _slider1;
			Titler _slider1Title = Titler(L"Font size slider", &_slider1);

			Slider _slider2;
			Titler _slider2Title = Titler(L"Margin slider", &_slider2);

			ProgressBar _progressBar;
			Titler _progressBarTitle = Titler(L"Progress bar", &_progressBar);

			Switch _switch;
			Titler _switchTitle = Titler(L"Switch", &_switch);

			TextField _textField;
			Titler textFieldTitle = Titler(L"Text", &_textField);

			Button _button;
			Titler _buttonTitle = Titler(L"Button", &_button);

			Button _toggleButton;
			Titler _toggleButtonTitle = Titler(L"Toggle button", &_toggleButton);
	};
}
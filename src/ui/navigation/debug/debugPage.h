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

			Slider _slider;
			Titler _sliderTitle = Titler(L"Title size slider", &_slider);

			Slider _slider2;
			Titler _slider2Title = Titler(L"Another slider", &_slider2);

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
#pragma once

#include "src/ui.h"
#include "theme.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class Switcher : public RelativeStackLayout {
		public:
			Switcher() {
				setOrientation(Orientation::horizontal);
				setSpacing(3);

				Theme::applyDescription(&_text);
				*this += &_text;

				Theme::apply(&_switch);
				setAutoSize(&_switch);
				*this += &_switch;
			}

			explicit Switcher(std::wstring_view title) : Switcher()  {
				_text.setText(title);
			}

			Switcher(std::wstring_view title, bool value) : Switcher(title) {
				_switch.setChecked(value);
			}

			Text& getText() {
				return _text;
			}

			Switch& getSwitch() {
				return _switch;
			}
		private:
			Text _text;
			Switch _switch;
	};
}
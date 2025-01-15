#pragma once

#include "../../../lib/YOBA/src/ui.h"
#include "../theme.h"

using namespace yoba;
using namespace yoba::ui;

namespace pizdanc {
	class Titler : public StackLayout {
		public:
			Titler() {
				setSpacing(3);

				_titleText.setPrimaryColor(&Theme::fg3);
				*this += &_titleText;
			}

			explicit Titler(const std::wstring_view& title) : Titler() {
				_titleText.setText(title);
			}

			Titler(const std::wstring_view& title, Element* element) : Titler(title) {
				*this += element;
			}

			void setText(const std::wstring_view& text) {
				_titleText.setText(text);
			}

		private:
			Text _titleText = Text();
	};
}
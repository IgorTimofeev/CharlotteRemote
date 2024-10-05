#pragma once

#include "../../../lib/YOBA/src/ui/text.h"
#include "../../../lib/YOBA/src/ui/stackLayout.h"
#include "../theme.h"

using namespace yoba;

namespace pizdanc {
	class Titler : public StackLayout {
		public:
			Titler() {
				setSpacing(3);

				_titleText.setForegroundColor(&Theme::fg3);
				*this += &_titleText;
			}

			explicit Titler(const wchar_t* title) : Titler() {
				_titleText.setText(title);
			}

			Titler(const wchar_t* title, Element* element) : Titler(title) {
				*this += element;
			}

			void setText(const wchar_t* text) {
				_titleText.setText(text);
			}

		private:
			Text _titleText = Text();
	};
}
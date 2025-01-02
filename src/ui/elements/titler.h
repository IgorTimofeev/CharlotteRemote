#pragma once

#include "../../../lib/YOBA/src/ui/text.h"
#include "../../../lib/YOBA/src/ui/stackContainer.h"
#include "../theme.h"

using namespace yoba;

namespace pizdanc {
	class Titler : public StackContainer {
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
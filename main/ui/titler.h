#pragma once

#include "src/ui.h"
#include "theme.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class Titler : public StackLayout {
		public:
			Titler() {
				setSpacing(3);

				_title.setFont(&Theme::fontNormal);
				_title.setTextColor(&Theme::fg3);
				*this += &_title;
			}

			explicit Titler(std::wstring_view title) : Titler()  {
				_title.setText(title);
			}

			Titler(std::wstring_view title, Element* element) : Titler(title) {
				*this += element;
			}

			Text& getTitle() {
				return _title;
			}

		private:
			Text _title = Text();
	};
}
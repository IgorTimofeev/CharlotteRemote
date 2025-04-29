#pragma once

#include <YOBA/UI.h>
#include "theme.h"

namespace pizda {
	using namespace YOBA;

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
			Text _title {};
	};
}
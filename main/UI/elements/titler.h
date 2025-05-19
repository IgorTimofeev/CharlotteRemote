#pragma once

#include <YOBA/UI.h>
#include <UI/theme.h>

namespace pizda {
	using namespace YOBA;

	class Titler : public StackLayout {
		public:
			Titler() {
				setSpacing(3);

				Theme::applyTitle(&_title);
				*this += &_title;
			}

			explicit Titler(std::wstring_view title) : Titler()  {
				_title.setText(title);
			}

			Titler(std::wstring_view title, Element* element) : Titler(title) {
				*this += element;
			}

			Titler(Element* element) : Titler() {
				*this += element;
			}

			Text& getTitle() {
				return _title;
			}

		private:
			Text _title {};
	};
}
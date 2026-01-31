#pragma once

#include <YOBA/UI.h>
#include "UI/theme.h"

namespace pizda {
	using namespace YOBA;

	class Titler : public StackLayout {
		public:
			Titler() {
				setSpacing(3);

				Theme::applyTitle(&title);
				*this += &title;
			}

			explicit Titler(std::wstring_view title) : Titler()  {
				this->title.setText(title);
			}

			Titler(std::wstring_view title, Element* element) : Titler(title) {
				*this += element;
			}

			explicit Titler(Element* element) : Titler() {
				*this += element;
			}

			TextView title {};
	};
}
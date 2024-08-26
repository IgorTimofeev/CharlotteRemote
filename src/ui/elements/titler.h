#pragma once

#include "yoba/elements/stack_layout.h"
#include "yoba/elements/element.h"
#include "yoba/elements/text.h"

using namespace yoba;

namespace pizdanc {
	class Titler : public StackLayout {
		public:
			Titler() {
				setSpacing(5);

				addChild(&_titleText);
			}

			explicit Titler(const String& title) : Titler() {
				_titleText.setText(title);
			}

		private:
			Text _titleText = Text();
	};
}
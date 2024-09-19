#pragma once

#include "../../../lib/YOBA/src/ui/text.h"
#include "../../../lib/YOBA/src/ui/stackLayout.h"
#include "../theme.h"

using namespace yoba;

namespace pizdanc {
	class Titler : public StackLayout {
		public:
			Titler() {
				setSpacing(5);

				_titleText.setForeground(&Theme::fg3);
				addChild(&_titleText);
			}

			explicit Titler(const wchar_t* title) : Titler() {
				_titleText.setText(title);
			}

			Titler(const wchar_t* title, Element* element) : Titler(title) {
				addChild(element);
			}

		private:
			Text _titleText = Text();
	};
}
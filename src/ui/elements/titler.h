#pragma once

#include "../../../lib/YOBA/src/ui/text.h"
#include "../../../lib/YOBA/src/ui/stackLayout.h"

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
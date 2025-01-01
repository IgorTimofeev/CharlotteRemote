#pragma once

#include "Arduino.h"

#include "../../../../lib/YOBA/src/ui/text.h"
#include "../../../../lib/YOBA/src/ui/textField.h"
#include "../../../../lib/YOBA/src/ui/slider.h"

#include "ui/navigation/page.h"
#include "ui/elements/titler.h"
#include "ui/elements/rows.h"

using namespace yoba;

namespace pizdanc {
	class DebugPage : public RowsPage {
		public:
			DebugPage();

			Slider slider = Slider();
			Titler sliderTitle = Titler(L"Slider", &slider);

			Button button = Button();
			Titler buttonTitle = Titler(L"Button", &button);

			TextField textField = TextField();
			Titler textFieldTitle = Titler(L"Text", &textField);
	};
}
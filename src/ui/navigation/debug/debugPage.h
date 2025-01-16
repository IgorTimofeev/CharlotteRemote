#pragma once

#include "Arduino.h"

#include "../../../../lib/yoba/src/main.h"
#include "../../../../lib/yoba/src/ui.h"

#include "ui/navigation/page.h"
#include "ui/elements/titler.h"
#include "ui/elements/rows.h"

namespace pizdanc {
	using namespace yoba;
	using namespace yoba::ui;

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
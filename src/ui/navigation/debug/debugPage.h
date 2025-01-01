#pragma once

#include "Arduino.h"

#include "../../../../lib/YOBA/src/ui/text.h"
#include "../../../../lib/YOBA/src/ui/slider.h"
#include "../../../../lib/YOBA/src/ui/keyboard.h"

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

			Button button1 = Button();
			Titler button1Title = Titler(L"Button", &button1);
	};
}
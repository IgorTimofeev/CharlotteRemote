#pragma once

#include "Arduino.h"

#include "../../../../lib/yoba/src/main.h"
#include "../../../../lib/yoba/src/ui.h"

#include "ui/navigation/page.h"

namespace pizdanc {
	using namespace yoba;
	using namespace yoba::ui;

	class DebugPage : public Page {
		public:
			DebugPage();

			ScrollView scrollView = ScrollView();

			StackLayout rows = StackLayout(Orientation::vertical, 10);

			Slider slider = Slider();
			Titler sliderTitle = Titler(L"Slider", &slider);

			TextField textField = TextField();
			Titler textFieldTitle = Titler(L"Text", &textField);

	};
}
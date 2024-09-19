#pragma once

#include "Arduino.h"

#include "../../../../lib/YOBA/src/ui/text.h"
#include "../../../../lib/YOBA/src/ui/slider.h"

#include "ui/navigation/page.h"
#include "ui/elements/titler.h"
#include "ui/elements/rows.h"

using namespace yoba;

namespace pizdanc {
	class RadioPage : public Page {
		public:
			RadioPage() {
				rows.setWidth(140);
				rows.setAlignment(Alignment::Center);

				// Slider
				Theme::apply(slider);
				slider.setValue(0);
				rows.addChild(&sliderTitle);

				// Button 1
				Theme::apply(button1);
				button1.setText(L"Click");
				rows.addChild(&button1Title);

				// Button 2
				Theme::apply(button2);
				button2.setToggle(true);
				button2.setText(L"Click");
				rows.addChild(&button2Title);

				addChild(&rows);
			}

			Rows rows = Rows();

			Slider slider = Slider();
			Titler sliderTitle = Titler(L"Slider", &slider);

			Button button1 = Button();
			Titler button1Title = Titler(L"Button", &button1);

			Button button2 = Button();
			Titler button2Title = Titler(L"Toggle button", &button2);

	};
}
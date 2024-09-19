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
				setAlignment(Alignment::Center);

				// Slider
				slider.setSize(Size(150, 40));
				slider.setBackground(&Theme::bg3);
				slider.setForeground(&Theme::yellow);
				slider.setCornerRadius(5);
				slider.setValue(0);

				slider.addOnValueChanged([this]() {

				});

				sliderTitle.addChild(&slider);
				rows.addChild(&sliderTitle);

				// Button
				button.setSize(Size(150, 40));
				button.setCornerRadius(5);

				button.setBackground(&Theme::yellow);
				button.setPressedBackground(&Theme::green);

				button.setForeground(&Theme::fg1);
				button.setPressedForeground(&Theme::fg4);

				button.setText(L"Penis button");

				button.addOnClick([&](TouchEvent& event) {
					Serial.printf("Click on %d x %d\n", event.getPosition().getX(), event.getPosition().getY());
				});

				buttonTitle.addChild(&button);
				rows.addChild(&buttonTitle);

				addChild(&rows);
			}

			Rows rows = Rows();

			Titler sliderTitle = Titler(L"Background");
			Slider slider = Slider();

			Titler buttonTitle = Titler(L"Press me");
			Button button = Button();
	};
}
#pragma once

#include "Arduino.h"

#include "yoba/elements/slider.h"
#include "yoba/elements/text.h"

#include "ui/navigation/page.h"
#include "ui/elements/titler.h"
#include "ui/elements/rows.h"

using namespace yoba;

namespace ui {
	class RadioPage : public Page {
		public:
			RadioPage() {
				setAlignment(Alignment::center);

				backgroundSlider.setCornerRadius(5);
				backgroundSlider.setValue(0);
				backgroundSlider.setSize(Size(150, 40));

				backgroundSlider.addOnValueChanged([this]() {

				});

				backgroundTitler.addChild(&backgroundSlider);

				rows.addChild(&backgroundTitler);

				addChild(&rows);
			}

			Rows rows = Rows();

			Titler backgroundTitler = Titler("Background");
			Slider backgroundSlider = Slider();
	};
}
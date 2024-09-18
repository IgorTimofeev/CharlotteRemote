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
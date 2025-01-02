#include "debugPage.h"
#include "../../../../lib/YOBA/src/ui/application.h"

namespace pizdanc {
	DebugPage::DebugPage() {
		rows.setWidth(140);
		rows.setAlignment(Alignment::Center);

		// Slider
		Theme::apply(slider);
		slider.setValue(0);
		rows += &sliderTitle;

		// Button 1
		Theme::apply(button);
		button.setText(U"Click");
		rows += &buttonTitle;

		// Text
		Theme::apply(textField);
		textField.setText(U"Hello world");
		rows += &textFieldTitle;
	}
}
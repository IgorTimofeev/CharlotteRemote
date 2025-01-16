#include "debugPage.h"
#include "ui/theme.h"

namespace pizdanc {
	DebugPage::DebugPage() {
		rows.setWidth(140);
		rows.setAlignment(Alignment::center);

		// Slider
		Theme::apply(slider);
		Theme::apply(sliderTitle);
		slider.setValue(0);
		rows += &sliderTitle;

		// Button 1
		Theme::apply(button);
		Theme::apply(buttonTitle);
		button.setText(L"Click");
		rows += &buttonTitle;

		// Text
		Theme::apply(textField);
		Theme::apply(textFieldTitle);
		textField.setText(L"Hello world pizda eblo ssanina penis chlen vagina");
		textField.setCursorToEnd();
		rows += &textFieldTitle;
	}
}
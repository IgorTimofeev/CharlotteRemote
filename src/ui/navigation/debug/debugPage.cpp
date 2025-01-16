#include "debugPage.h"
#include "ui/theme.h"
#include <sstream>

namespace pizdanc {
	DebugPage::DebugPage() {
		rows.setWidth(140);
		rows.setMargin(Margin(0, 20, 0, 20));
		rows.setHorizontalAlignment(Alignment::center);

		// Slider
		Theme::apply(&slider);
		Theme::apply(&sliderTitle);
		slider.setValue(0);
		rows += &sliderTitle;

		// Buttons
		static std::wstringstream stream;

		for (uint8_t i = 0; i < 5; i++) {
			stream.str(std::wstring());
			stream.clear();
			stream << L"Button ";
			stream << i;

			auto button = new Button[5];
			Theme::apply(button);
			button->setText(L"Click");

			auto buttonTitle = new Titler(stream.str(), button);
			Theme::apply(buttonTitle);

			rows += buttonTitle;
		}

		// Text
		Theme::apply(&textField);
		Theme::apply(&textFieldTitle);
		textField.setText(L"Hello world pizda eblo ssanina penis chlen vagina");
		textField.setCursorToEnd();
		rows += &textFieldTitle;

		scrollView += &rows;

		Theme::apply(&scrollView);
		*this += &scrollView;
	}
}
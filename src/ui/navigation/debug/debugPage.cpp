#include "debugPage.h"
#include "ui/theme.h"
#include <sstream>

namespace pizdanc {
	DebugPage::DebugPage() {
		auto scrollView = new ScrollView();

		auto rows = new StackLayout(Orientation::vertical, 10);

		rows->setMargin(Margin(60, 20));

		// Slider
		{
			auto slider = new Slider();
			Theme::apply(slider);
			slider->setValue(0.8f);

			auto sliderTitle = new Titler(L"Slider", slider);
			Theme::apply(sliderTitle);
			*rows += sliderTitle;
		}

		// Switch
		{
			auto sw = new Switch();
			Theme::apply(sw);

			auto swTitle = new Titler(L"Switch", sw);
			Theme::apply(swTitle);
			*rows += swTitle;
		}

		// Equal
		{
			auto row = new EqualStackLayout(Orientation::horizontal, 10);

			auto textField = new TextField();
			Theme::apply(textField);
			textField->setText(L"Hello world");
			textField->setCursorToEnd();
			*row += textField;

			auto button = new Button();
			Theme::apply(button);
			button->setWidth(36);
			button->setText(L".!.");
			row->setFit(button);
			*row += button;

			auto equalLayoutTitle = new Titler(L"Equal layout", row);
			Theme::apply(equalLayoutTitle);
			*rows += equalLayoutTitle;
		}

		// TextField
		{
			auto textField = new TextField();
			Theme::apply(textField);
			textField->setText(L"Hello world pizda eblo ssanina penis chlen vagina");
			textField->setCursorToEnd();

			auto textFieldTitle = new Titler(L"Text", textField);
			Theme::apply(textFieldTitle);

			*rows += textFieldTitle;
		}

		// Buttons
		{
			static std::wstringstream stream;

			for (uint8_t i = 0; i < 5; i++) {
				stream.str(std::wstring());
				stream << L"Button ";
				stream << i;

				auto button = new Button();
				Theme::apply(button);
				button->setText(L"Click");

				auto buttonTitle = new Titler(stream.str(), button);
				Theme::apply(buttonTitle);

				*rows += buttonTitle;
			}
		}

		*scrollView += rows;

		Theme::apply(scrollView);
		*this += scrollView;
	}
}
#include "debugPage.h"
#include "ui/theme.h"
#include "ui/rc.h"
#include <sstream>

namespace pizdanc {
	DebugPage::DebugPage() {
		auto scrollView = new ScrollView();

		_rows.setSpacing(10);
		_rows.setMargin(Margin(20, 20, 20, 20));

		// Page title
		Theme::applyPageTitle(&_pageTitle);
		_pageTitle.setText(L"Debug page");
		_rows += &_pageTitle;

		// Slider
		Theme::apply(&_slider);
		_slider.setValue(0.4f);

		_slider.valueChanged += [this]() {
			_pageTitle.setFontScale(1 + (uint8_t) std::round(_slider.getValue() * 12));
		};

		Theme::apply(&_sliderTitle);
		_rows += &_sliderTitle;

		// Progress bar
		Theme::apply(&_progressBar);
		_progressBar.setValue(0.8f);
		_progressBar.setFillColor(&Theme::bad2);

		Theme::apply(&_progressBarTitle);
		_rows += &_progressBarTitle;

		// Switch
		Theme::apply(&_switch);
		_switch.setCheckedColor(&Theme::sky);

		_switch.isCheckedChanged += [this]() {
			RC::getInstance().getDebugOverlay().setVisible(_switch.isChecked());
		};

		Theme::apply(&_switchTitle);
		_rows += &_switchTitle;

		// TextField
		Theme::apply(&_textField);
		_textField.setText(L"Hello world pizda eblo ssanina penis chlen vagina");
		_textField.setCursorToEnd();

		Theme::apply(&textFieldTitle);
		_rows += &textFieldTitle;

		// Button
		Theme::apply(&_button);
		_button.setText(L"Click me");

		Theme::apply(&_buttonTitle);
		_rows += &_buttonTitle;

		// Toggle button
		Theme::apply(&_toggleButton);
		_toggleButton.setToggle(true);
		_toggleButton.setDefaultBackgroundColor(&Theme::good2);
		_toggleButton.setPressedBackgroundColor(&Theme::good1);
		_toggleButton.setText(L"Change state");

		Theme::apply(&_toggleButtonTitle);
		_rows += &_toggleButtonTitle;

		*scrollView += &_rows;

		Theme::apply(scrollView);
		*this += scrollView;
	}
}
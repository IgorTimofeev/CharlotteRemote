#include "debugPage.h"
#include "ui/theme.h"
#include "ui/rc_application.h"
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
			_pageTitle.setFontScale(1 + (uint8_t) std::round(_slider.getValue() * 9));
		};

		Theme::apply(&_sliderTitle);
		_rows += &_sliderTitle;

		// Slider 2
		Theme::apply(&_slider2);
		_slider2.setValue(0.8f);
		_slider2.setActiveTrackColor(&Theme::bad2);

		Theme::apply(&_slider2Title);
		_rows += &_slider2Title;

		// Switch
		Theme::apply(&_switch);
		_switch.setCheckedTrackColor(&Theme::sky);

		_switch.isCheckedChanged += []() {
			auto& app = RCApplication::getInstance();

			app.setShowDebugInfo(!app.getShowDebugInfo());
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
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

		// Slider 1
		Theme::apply(&_slider1);
		_slider1.setValue(0.4f);

		_slider1.valueChanged += [this]() {
			_wrappedText.setFontScale(1 + (uint8_t) std::round(_slider1.getValue() * 8));
		};

		Theme::apply(&_slider1Title);
		_rows += &_slider1Title;

		// Slider 2
		Theme::apply(&_slider2);
		_slider2.setFillColor(&Theme::good2);
		_slider2.setValue(0.5f);

		_slider2.valueChanged += [this]() {
			uint16_t value = 1 + (uint8_t) std::round(_slider2.getValue() * 80);

			_rows.setMargin(Margin(value, _rows.getMargin().getTop(), value, _rows.getMargin().getTop()));
		};

		Theme::apply(&_slider2Title);
		_rows += &_slider2Title;

		// Wrapped text
		Theme::applyDescription(&_wrappedText);

		_wrappedText.setWrappingEnabled(true);

		_wrappedText.setText(
LR"(Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions)"
		);

		_rows += &_wrappedText;

		// Switch
		Theme::apply(&_switch);
		_switch.setCheckedColor(&Theme::sky);

		_switch.isCheckedChanged += [this]() {
			RC::getInstance().getDebugOverlay().setVisible(_switch.isChecked());
		};

		Theme::apply(&_switchTitle);
		_rows += &_switchTitle;

		// Progress bar
		Theme::apply(&_progressBar);
		_progressBar.setValue(0.8f);
		_progressBar.setFillColor(&Theme::bad2);

		Theme::apply(&_progressBarTitle);
		_rows += &_progressBarTitle;

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
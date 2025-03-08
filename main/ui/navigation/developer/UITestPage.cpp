#include "UITestPage.h"
#include "../../theme.h"
#include "../../../rc.h"
#include "../../dialog.h"

namespace pizda {
	UITestPage::UITestPage() {
		// Page title
		title.setText(L"Debug page");

		// Text font size slider
		Theme::apply(&_textFontSizeSlider);
		_textFontSizeSlider.setValue(0xFFFF * 40 / 100);

		_textFontSizeSlider.valueChanged += [this]() {
			_text.setFontScale(1 + (uint8_t) std::round(_textFontSizeSlider.getValue() * 8 / 0xFFFF));
		};

		rows += &_textSliderTitle;

		// Text margin slider
		Theme::apply(&_textMarginSlider);
		_textMarginSlider.setFillColor(&Theme::good2);
		_textMarginSlider.setValue(0xFFFF * 50 / 100);

		_textMarginSlider.valueChanged += [this]() {
			uint16_t value = 1 + (uint8_t) std::round(_textMarginSlider.getValue() * 80 / 0xFFFF);

			rows.setMargin(Margin(value, rows.getMargin().getTop(), value, rows.getMargin().getTop()));
		};

		rows += &_textMarginSliderTitle;

		// Text
		Theme::applyDescription(&_text);

		_text.setWrappingEnabled(true);

		_text.setText(
			LR"(Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions)"
		);

		rows += &_text;

		// Switch
		Theme::apply(&_switch);
		_switch.setCheckedColor(&Theme::sky);
		_switch.setChecked(RC::getInstance().isDebugOverlayVisible());

		_switch.isCheckedChanged += [this]() {
			RC::getInstance().setDebugOverlayVisibility(_switch.isChecked());
		};

		rows += &_switchTitle;

		// Progress bar
		Theme::apply(&_progressBar);
		_progressBar.setValue(0xFFFF * 80 / 100);
		_progressBar.setFillColor(&Theme::bad2);

		rows += &_progressBarTitle;

		// TextField
		Theme::apply(&_textField);
		_textField.setText(L"Hello world pizda eblo ssanina penis chlen vagina");
		_textField.setCursorToEnd();

		rows += &textFieldTitle;
	}
}
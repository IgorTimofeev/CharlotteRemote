#include "debugPage.h"
#include "../../../theme.h"
#include "../../../../rc.h"
#include <sstream>

namespace pizdanc {
	DebugPage::DebugPage() {
		_rows.setSpacing(10);
		_rows.setMargin(Margin(20, 20, 20, 20));

		// Page title
		Theme::applyPageTitle(&_pageTitle);
		_pageTitle.setText(L"Debug page");
		_rows += &_pageTitle;

		// Speaker frequency
		Theme::apply(&_speakerFrequencySlider);
		_speakerFrequencySlider.setValue(0.5f);

		Theme::apply(&_speakerFrequencySliderTitle);
		_rows += &_speakerFrequencySliderTitle;

		// Speaker duration
		Theme::apply(&_speakerDurationSlider);
		_speakerDurationSlider.setValue(0.5f);

		Theme::apply(&_speakerDurationSliderTitle);
		_rows += &_speakerDurationSliderTitle;

		// Speaker сщгте
		Theme::apply(&_speakerCountSlider);
		_speakerCountSlider.setValue(0.2f);

		Theme::apply(&_speakerCountSliderTitle);
		_rows += &_speakerCountSliderTitle;

		// Speaker button
		Theme::apply(&_speakerButton);
		_speakerButton.setText(L"Play sound");
		_speakerButton.pressedChanged += [this]() {
			if (_speakerButton.isPressed())
				return;

			auto& rc = RC::getInstance();

			const auto frequency = (uint32_t) (_speakerFrequencySlider.getValue() * (float) 10000);
			const auto duration = (uint32_t) (_speakerDurationSlider.getValue() * (float) 500 * 1000);
			const auto count = (uint8_t) (_speakerCountSlider.getValue() * (float) 10);

			ESP_LOGI("Debug", "Speaker test: %lu, %lu, %d", frequency, duration, count);

			std::vector<Note> notes {};

			for (uint8_t i = 0; i < count; i++) {
				notes.emplace_back(frequency, duration);
				notes.push_back(Delay(duration));
			}

			rc.getSpeaker().play(Sound(notes));
		};

		_rows += &_speakerButton;

		// Text font size slider
		Theme::apply(&_textFontSizeSlider);
		_textFontSizeSlider.setValue(0.4f);

		_textFontSizeSlider.valueChanged += [this]() {
			_text.setFontScale(1 + (uint8_t) std::round(_textFontSizeSlider.getValue() * 8));
		};

		Theme::apply(&_textSliderTitle);
		_rows += &_textSliderTitle;

		// Text margin slider
		Theme::apply(&_textMarginSlider);
		_textMarginSlider.setFillColor(&Theme::good2);
		_textMarginSlider.setValue(0.5f);

		_textMarginSlider.valueChanged += [this]() {
			uint16_t value = 1 + (uint8_t) std::round(_textMarginSlider.getValue() * 80);

			_rows.setMargin(Margin(value, _rows.getMargin().getTop(), value, _rows.getMargin().getTop()));
		};

		Theme::apply(&_textMarginSliderTitle);
		_rows += &_textMarginSliderTitle;

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

		_rows += &_text;

		// Switch
		Theme::apply(&_switch);
		_switch.setCheckedColor(&Theme::sky);
		_switch.setChecked(RC::getInstance().getSettings().debugInfoVisible);

		_switch.isCheckedChanged += [this]() {
			auto& rc = RC::getInstance();

			auto& settings = rc.getSettings();
			settings.debugInfoVisible = _switch.isChecked();
			settings.enqueueWrite();

			rc.updateDebugInfoVisibility();
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

		_scrollView += &_rows;

		Theme::apply(&_scrollView);
		*this += &_scrollView;
	}
}
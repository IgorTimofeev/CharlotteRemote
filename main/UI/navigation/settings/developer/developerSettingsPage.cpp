#include "developerSettingsPage.h"
#include "../../../theme.h"
#include "../../../../rc.h"
#include "../../../elements/dialog.h"

namespace pizda {
	DeveloperSettingsPage::DeveloperSettingsPage() {
		// Page title
		title.setText(L"Developer page");

		// Debug overlay switch
		Theme::apply(&_debugOverlaySwitch);
		_debugOverlaySwitch.setCheckedColor(&Theme::sky);
		_debugOverlaySwitch.setChecked(RC::getInstance().getSettings().interface.developer.debugOverlay);

		_debugOverlaySwitch.isCheckedChanged += [this] {
			auto& settings = RC::getInstance().getSettings();
			settings.interface.developer.debugOverlay = _debugOverlaySwitch.isChecked();
			settings.enqueueWrite();

			RC::getInstance().updateDebugOverlayVisibility();
		};

		rows += &_debugOverlaySwitchTitle;

		// Speaker
		// Title
		title.setText(L"Speaker");

		// Frequency
		Theme::apply(&_speakerFrequencySlider);
		_speakerFrequencySlider.setFillColor(&Theme::good2);
		_speakerFrequencySlider.setValue(0xFFFF * 50 / 100);

		rows += &_speakerFrequencySliderTitle;

		// Duration
		_speakerDurationSlider.setFillColor(&Theme::bad2);
		Theme::apply(&_speakerDurationSlider);
		_speakerDurationSlider.setValue(0xFFFF * 50 / 100);

		rows += &_speakerDurationSliderTitle;

		// Count
		Theme::apply(&_speakerCountSlider);
		_speakerCountSlider.setValue(0xFFFF * 20 / 100);

		rows += &_speakerCountSliderTitle;

		// Button
		Theme::apply(&_speakerButton);
		_speakerButton.setText(L"Play");

		_speakerButton.click += [this] {
			auto& rc = RC::getInstance();

			const uint32_t frequency = static_cast<uint32_t>(_speakerFrequencySlider.getValue()) * 12'000ul / 0xFFFFul;
			const uint32_t duration = static_cast<uint64_t>(_speakerDurationSlider.getValue()) * 1'000'000ull / 0xFFFFull;
			const auto count = static_cast<uint8_t>(1 + static_cast<uint32_t>(_speakerCountSlider.getValue()) * 5ul / 0xFFFFul);

			ESP_LOGI("Debug", "Speaker test: %lu, %lu, %d", frequency, duration, count);

			std::vector<Note> notes {};

			for (uint8_t i = 0; i < count; i++) {
				notes.emplace_back(frequency, duration);
				notes.push_back(Delay(duration));
			}

			rc.getSpeaker().play(Sound(notes));
		};

		rows += &_speakerButton;

		// Text font size slider
		Theme::apply(&_textFontSizeSlider);
		_textFontSizeSlider.setValue(0xFFFF * 40 / 100);

		_textFontSizeSlider.valueChanged += [this] {
			_text.setFontScale(1 + static_cast<uint8_t>(std::round(_textFontSizeSlider.getValue() * 8 / 0xFFFF)));
		};

		rows += &_textSliderTitle;

		// Text margin slider
		Theme::apply(&_textMarginSlider);
		_textMarginSlider.setFillColor(&Theme::good2);
		_textMarginSlider.setValue(0xFFFF * 50 / 100);

		_textMarginSlider.valueChanged += [this] {
			const uint16_t value = 1 + static_cast<uint8_t>(std::round(_textMarginSlider.getValue() * 80 / 0xFFFF));

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
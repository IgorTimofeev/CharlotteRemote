#include "debugPage.h"
#include "../../theme.h"
#include "../../../rc.h"
#include "../../dialog.h"
#include "../../../hardware/OTA.h"

namespace pizda {
	DebugPage::DebugPage() {
		// Page title
		title.setText(L"Debug page");

		// OTA button
		Theme::apply(&_OTAButton);
		_OTAButton.setText(L"OTA");

		_OTAButton.pressedChanged += [this]() {
			if (_OTAButton.isPressed())
				return;

			auto& rc = RC::getInstance();

			auto dialog = new ProgressDialog();
			dialog->title.setText(L"Updating firmware");
			dialog->description.setText(L"Do not turn off your device and wait for the process to finish. Otherwise, you risk getting a fucking awesome but poorly functional brick.");
			dialog->show(&rc.getApplication());

			auto ota = new OTA();

			ota->onProgressChanged += [dialog](uint16_t progress) {
				dialog->setProgress(progress);
			};

			ota->onFinished += []() {
				esp_restart();
			};

			ota->start();
		};

		rows += &_OTAButton;

		// Speaker frequency
		Theme::apply(&_speakerFrequencySlider);
		_speakerFrequencySlider.setValue(0xFFFF * 50 / 100);

		rows += &_speakerFrequencySliderTitle;

		// Speaker duration
		Theme::apply(&_speakerDurationSlider);
		_speakerDurationSlider.setValue(0xFFFF * 50 / 100);

		rows += &_speakerDurationSliderTitle;

		// Speaker
		Theme::apply(&_speakerCountSlider);
		_speakerCountSlider.setValue(0xFFFF * 20 / 100);

		rows += &_speakerCountSliderTitle;

		// Speaker button
		Theme::apply(&_speakerButton);
		_speakerButton.setText(L"Play sound");

		_speakerButton.pressedChanged += [this]() {
			if (_speakerButton.isPressed())
				return;

			auto& rc = RC::getInstance();

			const uint32_t frequency = (uint32_t) _speakerFrequencySlider.getValue() * 15'000ul / 0xFFFFul;
			const uint32_t duration = (uint64_t) _speakerDurationSlider.getValue() * 2'000'000ull / 0xFFFFull;
			const auto count = (uint8_t) (1 + (uint32_t)_speakerCountSlider.getValue() * 10ul / 0xFFFFul);

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
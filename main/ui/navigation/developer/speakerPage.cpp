#include "speakerPage.h"

#include "../../../rc.h"
#include "../../theme.h"
#include "../../../constants.h"
#include "../../../sound.h"

namespace pizdanc {

}

pizda::SpeakerPage::SpeakerPage() {
	// Title
	title.setText(L"Speaker");

	// Frequency
	Theme::apply(&_frequencySlider);
	_frequencySlider.setFillColor(&Theme::good2);
	_frequencySlider.setValue(0xFFFF * 50 / 100);

	rows += &_frequencySliderTitle;

	// Duration
	_durationSlider.setFillColor(&Theme::bad2);
	Theme::apply(&_durationSlider);
	_durationSlider.setValue(0xFFFF * 50 / 100);

	rows += &_durationSliderTitle;

	// Count
	Theme::apply(&_countSlider);
	_countSlider.setValue(0xFFFF * 20 / 100);

	rows += &_countSliderTitle;

	// Button
	Theme::apply(&_button);
	_button.setText(L"Play");

	_button.isCheckedChanged += [this]() {
		if (_button.isChecked())
			return;

		auto& rc = RC::getInstance();

		const uint32_t frequency = (uint32_t) _frequencySlider.getValue() * 12'000ul / 0xFFFFul;
		const uint32_t duration = (uint64_t) _durationSlider.getValue() * 1'000'000ull / 0xFFFFull;
		const auto count = (uint8_t) (1 + (uint32_t)_countSlider.getValue() * 5ul / 0xFFFFul);

		ESP_LOGI("Debug", "Speaker test: %lu, %lu, %d", frequency, duration, count);

		std::vector<Note> notes {};

		for (uint8_t i = 0; i < count; i++) {
			notes.emplace_back(frequency, duration);
			notes.push_back(Delay(duration));
		}

		rc.getSpeaker().play(Sound(notes));
	};

	rows += &_button;
}

#include "spectrumScanPage.h"

#include <format>

#include "rc.h"
#include "UI/theme.h"
#include "utils/string.h"

namespace pizda {
	void SpectrumScanningChart::onTick() {
		Control::onTick();

		if (RC::getInstance().getRemoteData().radio.spectrumScanning.state != RemoteDataRadioSpectrumScanningState::none)
			invalidate();
	}

	void SpectrumScanningChart::onRender(Renderer* renderer, const Bounds& bounds) {
		auto& ss = RC::getInstance().getRemoteData().radio.spectrumScanning;

		renderer->renderFilledRectangle(
			bounds,
			&Theme::bg2
		);

		// Axes
		renderer->renderHorizontalLine(
			bounds.getBottomLeft(),
			bounds.getWidth(),
			&Theme::fg4
		);

		renderer->renderVerticalLine(
			bounds.getTopLeft(),
			bounds.getHeight() - 1,
			&Theme::fg4
		);

		// History
		constexpr static int8_t RSSIMin = -100;
		constexpr static int8_t RSSIMax = 0;

		constexpr static int8_t colorMapLength = 5;
		constexpr static const Color* colorMap[colorMapLength] {
			&Theme::magenta,
			&Theme::ocean,
			&Theme::green1,
			&Theme::yellow,
			&Theme::red
		};

		const uint64_t frequencyDelta = ss.frequency.to - ss.frequency.from;

		for (uint16_t localX = 0; localX < bounds.getWidth(); localX++) {
			const auto frequency =
				static_cast<uint64_t>(ss.frequency.from)
				+ static_cast<uint64_t>(localX)
				* frequencyDelta
				/ static_cast<uint64_t>(bounds.getWidth());

			const auto historyIndex = static_cast<uint64_t>(localX) * static_cast<uint64_t>(ss.history.size()) / static_cast<uint64_t>(bounds.getWidth());

			if (frequency < ss.frequency.value) {
				const auto RSSI = std::clamp<int8_t>(ss.history[historyIndex], RSSIMin, RSSIMax);

				const auto colorIndex = std::min(
					std::abs(RSSI - RSSIMin) * colorMapLength / std::abs(RSSIMax - RSSIMin),
					colorMapLength - 1
				);

				const auto color = colorMap[colorIndex];
				const auto lineHeight = std::abs(RSSI - RSSIMin) * bounds.getHeight() / std::abs(RSSIMax - RSSIMin);

				renderer->renderVerticalLine(
					Point(bounds.getX() + localX, bounds.getY2() - lineHeight),
					lineHeight,
					color
				);
			}
			else {
				renderer->renderVerticalLine(
					Point(bounds.getX() + localX, bounds.getY()),
					bounds.getHeight(),
					&Theme::fg1
				);

				break;
			}
		}
	}

	SpectrumScanPage::SpectrumScanPage() {
		// Page title
		title.setText(L"Spectrum scanning");

		// Char
		chart.setHeight(180);
		rows += &chart;

		// -------------------------------- Frequency --------------------------------

		frequencyRow.setOrientation(Orientation::horizontal);
		frequencyRow.setSpacing(5);
		rows += &frequencyRow;

		// From
		Theme::apply(&frequencyFromTextField);
		frequencyFromTextField.setText(L"909");
		frequencyRow += &frequencyFromTitle;

		// To
		Theme::apply(&frequencyToTextField);
		frequencyToTextField.setText(L"918");
		frequencyRow += &frequencyToTitle;

		// Step
		Theme::apply(&frequencyStepTextField);
		frequencyStepTextField.setText(L"10");
		frequencyRow += &frequencyStepTitle;

		// Start button
		Theme::applyPrimary(&startButton);
		startButton.setText(L"Scan");

		startButton.click += [this] {
			auto& rc = RC::getInstance();

			tryParse();

			rc.getRemoteData().radio.spectrumScanning.frequency.from = frequencyFrom;
			rc.getRemoteData().radio.spectrumScanning.frequency.to = frequencyTo;
			rc.getRemoteData().radio.spectrumScanning.frequency.step = frequencyStep;
			rc.getRemoteData().radio.spectrumScanning.frequency.value = frequencyFrom;

			// Clearing history
			for (auto& value : rc.getRemoteData().radio.spectrumScanning.history)
				value = 0;

			rc.getRemoteData().radio.spectrumScanning.state = RemoteDataRadioSpectrumScanningState::requested;
		};

		rows += &startButton;

		tryParse();
	}

	void SpectrumScanPage::onTick() {
		Layout::onTick();

		auto& rc = RC::getInstance();

		startButton.setEnabled(rc.getRemoteData().radio.spectrumScanning.state == RemoteDataRadioSpectrumScanningState::none);
	}

	void SpectrumScanPage::tryParse() {
		int32_t result;

		if (StringUtils::tryParseInt32(frequencyFromTextField.getText(), result))
			frequencyFrom = std::clamp<uint32_t>(result, 0, 1000) * 1'000'000;

		if (StringUtils::tryParseInt32(frequencyToTextField.getText(), result))
			frequencyTo = std::clamp<uint32_t>(result, 0, 1000) * 1'000'000;

		if (frequencyTo < frequencyFrom)
			std::swap(frequencyFrom, frequencyTo);

		if (StringUtils::tryParseInt32(frequencyStepTextField.getText(), result))
			frequencyStep = std::clamp<uint32_t>(result, 0, 1000) * 1'000;

		ESP_LOGI("SpectrumScanPage::tryParse()", "from: %lu, to: %lu, step %lu", frequencyFrom, frequencyTo, frequencyStep);
	}
}

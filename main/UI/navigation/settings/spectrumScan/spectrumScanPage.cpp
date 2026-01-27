#include "spectrumScanPage.h"

#include <format>
#include <algorithm>

#include "rc.h"
#include "UI/theme.h"
#include "utils/string.h"

namespace pizda {
	void SpectrumScanningChart::onTick() {
		Control::onTick();

		if (RC::getInstance().getRemoteData().radio.spectrumScanning.state != RemoteDataRadioSpectrumScanningState::none)
			invalidate();
	}

	void SpectrumScanningChart::onEvent(Event* event) {
		Control::onEvent(event);

		if (event->getTypeID() == PointerDownEvent::typeID) {
			setCaptured(true);

			updatePointerPos(reinterpret_cast<PointerDownEvent*>(event)->getPosition());
			invalidate();

			event->setHandled(true);
		}
		else if (event->getTypeID() == PointerDragEvent::typeID) {
			updatePointerPos(reinterpret_cast<PointerDragEvent*>(event)->getPosition());
			invalidate();

			event->setHandled(true);
		}
		else if (event->getTypeID() == PointerUpEvent::typeID) {
			setCaptured(false);

			invalidate();

			event->setHandled(true);
		}
	}

	void SpectrumScanningChart::onRender(Renderer* renderer, const Bounds& bounds) {
		constexpr static int8_t RSSIMin = -100;
		constexpr static int8_t RSSIMax = 0;

		constexpr static uint8_t textHOffset = 4;
		constexpr static uint8_t textVOffset = 1;

		constexpr static int8_t colorMapLength = 5;
		constexpr static const Color* colorMap[colorMapLength] {
			&Theme::magenta,
			&Theme::ocean,
			&Theme::green1,
			&Theme::yellow,
			&Theme::red
		};

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
		const auto& getFrequency = [&ss, &bounds](const uint16_t localX) {
			return
				static_cast<uint64_t>(ss.frequency.from)
				+ static_cast<uint64_t>(localX)
				* static_cast<uint64_t>(ss.frequency.to - ss.frequency.from)
				/ static_cast<uint64_t>(bounds.getWidth());
		};

		const auto& getHistoryIndex = [&ss, &bounds](const uint16_t localX) {
			return
				static_cast<uint64_t>(localX)
				* static_cast<uint64_t>(ss.history.size())
				/ static_cast<uint64_t>(bounds.getWidth());
		};

		const auto& getRSSI = [&ss](const uint16_t historyIndex) {
			return std::clamp<int8_t>(ss.history[historyIndex], RSSIMin, RSSIMax);
		};

		const auto& getRSSIHeight = [&bounds](const int8_t RSSI) {
			return std::abs(RSSI - RSSIMin) * bounds.getHeight() / std::abs(RSSIMax - RSSIMin);
		};

		if (_pointerPos.getX() < 0)
			_pointerPos = bounds.getCenter();

		// Pointer
		{
			const auto frequency = getFrequency(_pointerPos.getX());

			// Horizontal
			const auto pointerRSSI = RSSIMin + _pointerPos.getY() * (RSSIMax - RSSIMin) / bounds.getHeight();
			auto text = std::format(L"{} dBm", pointerRSSI);
			auto textWidth = Theme::fontSmall.getWidth(text);

			renderer->renderString(
				Point(
					bounds.getX() + textHOffset,
					bounds.getY() + _pointerPos.getY() - Theme::fontSmall.getHeight() / 2
				),
				&Theme::fontSmall,
				&Theme::fg4,
				text
			);

			renderer->renderHorizontalLine(
				Point(
					bounds.getX() + textHOffset + textWidth + textHOffset,
					bounds.getY() + _pointerPos.getY()
				),
				bounds.getWidth() - (textHOffset + textWidth + textHOffset),
				&Theme::fg4
			);

			// Vertical
			text = std::format(L"{} MHz", frequency / 1'000'000);
			textWidth = Theme::fontSmall.getWidth(text);

			renderer->renderString(
				Point(
					bounds.getX() + _pointerPos.getX() - textWidth / 2,
					bounds.getY() + textVOffset
				),
				&Theme::fontSmall,
				&Theme::fg4,
				text
			);

			renderer->renderVerticalLine(
				Point(
					bounds.getX() + _pointerPos.getX(),
					bounds.getY() + textVOffset + Theme::fontSmall.getHeight() + textVOffset
				),
				bounds.getHeight() - (textVOffset + Theme::fontSmall.getHeight() + textVOffset),
				&Theme::fg4
			);
		}

		// History
		for (uint16_t localX = 0; localX < bounds.getWidth(); localX++) {
			const auto frequency = getFrequency(localX);
			const auto historyIndex = getHistoryIndex(localX);

			if (frequency < ss.frequency.value) {
				const auto RSSI = getRSSI(historyIndex);
				const auto lineHeight = getRSSIHeight(RSSI);

				const auto colorIndex = std::min(
					std::abs(RSSI - RSSIMin) * colorMapLength / std::abs(RSSIMax - RSSIMin),
					colorMapLength - 1
				);

				const auto color = colorMap[colorIndex];

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

		// Tip
		{
			const auto historyIndex = getHistoryIndex(_pointerPos.getX());
			const auto historyRSSI = getRSSI(historyIndex);

			if (historyRSSI > RSSIMin) {
				const auto historyRSSIHeight = getRSSIHeight(historyRSSI);

				const auto tipY = bounds.getY2() - historyRSSIHeight;

				renderer->renderFilledCircle(
					Point(
						bounds.getX() + _pointerPos.getX(),
						tipY
					),
					3,
					&Theme::fg1
				);

				const auto text = std::format(L"{} dBm", historyRSSI);

				renderer->renderString(
					Point(
						bounds.getX() + _pointerPos.getX() + textHOffset,
						tipY - Theme::fontSmall.getHeight() / 2
					),
					&Theme::fontSmall,
					&Theme::fg1,
					text
				);
			}
		}
	}

	void SpectrumScanningChart::updatePointerPos(const Point& pointerEventPos) {
		const auto& bounds = getBounds();

		_pointerPos = pointerEventPos - bounds.getTopLeft();
		_pointerPos.setX(std::clamp<int32_t>(_pointerPos.getX(), 0, bounds.getWidth() - 1));
		_pointerPos.setY(std::clamp<int32_t>(_pointerPos.getY(), 0, bounds.getHeight() - 1));
	}

	SpectrumScanPage::SpectrumScanPage() {
		// Page title
		title.setText(L"Spectrum scanning");

		// Char
		chart.setHeight(170);
		rows += &chart;

		// -------------------------------- Frequency --------------------------------

		frequencyRow.setOrientation(Orientation::horizontal);
		frequencyRow.setSpacing(5);
		rows += &frequencyRow;

		// From
		Theme::apply(&frequencyFromTextField);
		frequencyFromTextField.setText(L"430");
		frequencyRow += &frequencyFromTitle;

		// To
		Theme::apply(&frequencyToTextField);
		frequencyToTextField.setText(L"440");
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
			rc.getRemoteData().radio.spectrumScanning.state = RemoteDataRadioSpectrumScanningState::requested;
		};

		rows += &startButton;

		// Clearing history
		for (auto& value : RC::getInstance().getRemoteData().radio.spectrumScanning.history)
			value = std::numeric_limits<int8_t>::min();

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

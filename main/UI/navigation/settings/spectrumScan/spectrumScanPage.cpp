#include "spectrumScanPage.h"

#include <format>
#include <algorithm>

#include "rc.h"
#include "UI/theme.h"
#include "utils/string.h"

namespace pizda {
	void SpectrumScanningChart::onTick() {
		Control::onTick();

		if (RC::getInstance().getRemoteData().transceiver.spectrumScanning.state != RemoteDataRadioSpectrumScanningState::none)
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

		constexpr static std::array<const Color*, 5> colorMap {
			&Theme::blue,
			&Theme::ocean,
			&Theme::green1,
			&Theme::yellow,
			&Theme::red
		};

		auto& rc = RC::getInstance();
		auto& st = rc.getSettings().transceiver.spectrumScanning;
		auto& rd = RC::getInstance().getRemoteData().transceiver.spectrumScanning;

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
		const auto& getFrequency = [&st, &bounds](const uint16_t localX) {
			return
				static_cast<uint64_t>(st.frequency.from)
				+ static_cast<uint64_t>(localX)
				* static_cast<uint64_t>(st.frequency.to - st.frequency.from)
				/ static_cast<uint64_t>(bounds.getWidth());
		};

		const auto& getHistoryIndex = [&rd, &bounds](const uint16_t localX) {
			return
				static_cast<uint64_t>(localX)
				* static_cast<uint64_t>(rd.history.size())
				/ static_cast<uint64_t>(bounds.getWidth());
		};

		const auto& getRSSI = [&rd](const uint16_t historyIndex) {
			return std::clamp<int8_t>(rd.history[historyIndex].RSSI, RSSIMin, RSSIMax);
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
		auto scanningLineNotRendered = true;

		for (uint16_t localX = 0; localX < bounds.getWidth(); localX++) {
			const auto frequency = getFrequency(localX);
			const auto historyIndex = getHistoryIndex(localX);
			const auto RSSI = getRSSI(historyIndex);
			const auto lineHeight = getRSSIHeight(RSSI);

			const Color* color;

			if (frequency <= rd.frequency) {
				const auto colorIndex = std::min<uint32_t>(
					static_cast<uint32_t>(rd.history[historyIndex].saturation) * colorMap.size() / 0xFF,
					colorMap.size() - 1
				);

				color = colorMap[colorIndex];
			}
			else {
				color = &Theme::fg6;
			}

			renderer->renderVerticalLine(
				Point(bounds.getX() + localX, bounds.getY2() - lineHeight),
				lineHeight,
				color
			);

			if (scanningLineNotRendered && frequency > rd.frequency) {
				renderer->renderVerticalLine(
					Point(bounds.getX() + localX, bounds.getY()),
					bounds.getHeight(),
					&Theme::fg1
				);

				scanningLineNotRendered = false;
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
		frequencyFromTextField.setText(std::to_wstring(RC::getInstance().getSettings().transceiver.spectrumScanning.frequency.from / 1'000'000));
		frequencyRow += &frequencyFromTitle;

		// To
		Theme::apply(&frequencyToTextField);
		frequencyToTextField.setText(std::to_wstring(RC::getInstance().getSettings().transceiver.spectrumScanning.frequency.to / 1'000'000));
		frequencyRow += &frequencyToTitle;

		// Step
		Theme::apply(&frequencyStepTextField);
		frequencyStepTextField.setText(std::to_wstring(RC::getInstance().getSettings().transceiver.spectrumScanning.frequency.step / 1'000));
		frequencyRow += &frequencyStepTitle;

		// Start button
		Theme::applyPrimary(&beginButton);
		beginButton.setText(L"Begin");

		// -------------------------------- Shortcuts --------------------------------

		frequencyShortcutRow.setOrientation(Orientation::horizontal);
		frequencyShortcutRow.setSpacing(5);
		rows += &frequencyShortcutRow;

		const auto growPodzalupnik = [this](Button& button, const std::wstring_view buttonText, const std::wstring_view fromText, const std::wstring_view toText) {
			Theme::applySecondary(&button);

			button.setText(buttonText);

			button.click += [this, fromText, toText] {
				frequencyFromTextField.setText(fromText);
				frequencyToTextField.setText(toText);
			};

			frequencyShortcutRow += &button;
		};

		growPodzalupnik(frequencyShortcut430_440Button, L"430", L"430", L"440");
		growPodzalupnik(frequencyShortcut470_510Button, L"470", L"470", L"510");
		growPodzalupnik(frequencyShortcut779_787Button, L"779", L"779", L"787");
		growPodzalupnik(frequencyShortcut863_870Button, L"863", L"863", L"870");
		growPodzalupnik(frequencyShortcut902_928Button, L"902", L"902", L"928");

		// -------------------------------- Begin button --------------------------------

		beginButton.click += [this] {
			auto& rc = RC::getInstance();

			int32_t result;

			if (StringUtils::tryParseInt32(frequencyFromTextField.getText(), result))
				rc.getSettings().transceiver.spectrumScanning.frequency.from = std::clamp<uint32_t>(result, 0, 1000) * 1'000'000;

			if (StringUtils::tryParseInt32(frequencyToTextField.getText(), result))
				rc.getSettings().transceiver.spectrumScanning.frequency.to = std::clamp<uint32_t>(result, 0, 1000) * 1'000'000;

			if (rc.getSettings().transceiver.spectrumScanning.frequency.from > rc.getSettings().transceiver.spectrumScanning.frequency.to)
				std::swap(rc.getSettings().transceiver.spectrumScanning.frequency.from, rc.getSettings().transceiver.spectrumScanning.frequency.to);

			if (StringUtils::tryParseInt32(frequencyStepTextField.getText(), result))
				rc.getSettings().transceiver.spectrumScanning.frequency.step = std::clamp<uint32_t>(result, 0, 1000) * 1'000;

			rc.getSettings().transceiver.scheduleWrite();

			rc.getRemoteData().transceiver.spectrumScanning.frequency = rc.getSettings().transceiver.spectrumScanning.frequency.from;
			rc.getRemoteData().transceiver.spectrumScanning.state = RemoteDataRadioSpectrumScanningState::requested;
		};

		rows += &beginButton;

		// -------------------------------- Initialization --------------------------------

		// Clearing history
		for (auto& value : RC::getInstance().getRemoteData().transceiver.spectrumScanning.history) {
			value.RSSI = std::numeric_limits<int8_t>::min();
			value.saturation = 0;
		}
	}

	void SpectrumScanPage::onTick() {
		Layout::onTick();

		auto& rc = RC::getInstance();

		beginButton.setEnabled(rc.getRemoteData().transceiver.spectrumScanning.state == RemoteDataRadioSpectrumScanningState::none);
	}

}

#include "spectrumScanPage.h"

#include <format>
#include <algorithm>

#include "rc.h"
#include "UI/theme.h"
#include "utilities/string.h"

namespace pizda {
	SpectrumScanningFrequencyPresetsDialog::SpectrumScanningFrequencyPresetsDialog(const std::function<void(const std::wstring_view from, const std::wstring_view to)>& onConfirm): onConfirm(onConfirm) {
		title.setText(L"Presets");

		growPodzalupnik(button430_440, L"430 - 440 MHz", L"430", L"440");
		growPodzalupnik(button470_510, L"470 - 510 MHz", L"470", L"510");
		growPodzalupnik(button779_787, L"779 - 787 MHz", L"779", L"787");
		growPodzalupnik(button863_870, L"863 - 870 MHz", L"863", L"870");
		growPodzalupnik(button902_928, L"902 - 928 MHz", L"902", L"928");
	}

	void SpectrumScanningFrequencyPresetsDialog::growPodzalupnik(Button& button, const std::wstring_view buttonText, const std::wstring_view from, const std::wstring_view to) {
		Theme::applySecondary(&button);
		button.setText(buttonText);

		button.setOnClick([this, to, from] {
			RC::getInstance().getApplication().invokeOnNextTick([this, to, from] {
				onConfirm(from, to);

				hide();
				delete this;
			});
		});

		rows += &button;
	}

	void SpectrumScanningChart::onTick() {
		Control::onTick();

		if (RC::getInstance().getRemoteData().transceiver.spectrumScanning.state != RemoteDataRadioSpectrumScanningState::stopped)
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
		else if (event->getTypeID() == PinchDownEvent::typeID) {
			_pinchLength = reinterpret_cast<PinchDownEvent*>(event)->getLength();

			event->setHandled(true);
		}
		else if (event->getTypeID() == PinchDragEvent::typeID) {
			const auto pinchLength = reinterpret_cast<PinchDragEvent*>(event)->getLength();
			const auto pinchDelta = pinchLength - _pinchLength;
			_pinchLength = pinchLength;

			_RSSIMax = std::clamp<int8_t>(_RSSIMax - pinchDelta / 2.f, _RSSIMin + 10, 0);

			invalidate();

			event->setHandled(true);
		}
	}

	void SpectrumScanningChart::onRender(Renderer* renderer, const Bounds& bounds) {
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

		// Background
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

		const auto& getRSSI = [&rd, this](const uint16_t historyIndex) {
			return std::clamp<int8_t>(rd.history[historyIndex], _RSSIMin, _RSSIMax);
		};

		const auto& getRSSIHeight = [&bounds, this](const int8_t RSSI) {
			return std::abs(RSSI - _RSSIMin) * bounds.getHeight() / std::abs(_RSSIMax - _RSSIMin);
		};

		// History
		auto shouldRenderScanline = rd.frequency > 0;

		for (uint16_t localX = 0; localX < bounds.getWidth(); localX++) {
			const auto frequency = getFrequency(localX);

			if (shouldRenderScanline && frequency > rd.frequency) {
				renderer->renderVerticalLine(
					Point(bounds.getX() + localX, bounds.getY()),
					bounds.getHeight(),
					&Theme::fg1
				);

				shouldRenderScanline = false;
			}
			else {
				const auto historyIndex = getHistoryIndex(localX);
				const auto RSSI = getRSSI(historyIndex);
				const auto lineHeight = getRSSIHeight(RSSI);

				if (lineHeight > 0) {
					const Color* color;

					if (frequency < rd.frequency) {
						const auto colorIndex = std::min<uint32_t>(
							 rd.historyMax == rd.historyMin
								? 0
								: static_cast<uint32_t>(std::abs(rd.history[historyIndex] - rd.historyMin))
									* colorMap.size()
									/ std::abs(rd.historyMax - rd.historyMin),
							colorMap.size() - 1
						);

						color = colorMap[colorIndex];
					}
					else {
						color = &Theme::fg6;
					}

					renderer->renderVerticalLine(
						Point(bounds.getX() + localX, bounds.getY2() - lineHeight + 1),
						lineHeight,
						color
					);
				}
			}
		}

		// Pointer
		{
			const auto pointerPixelPos = Point(
				_pointerPos.getX() * bounds.getWidth(),
				_pointerPos.getY() * bounds.getHeight()
			);

			const auto frequency = getFrequency(pointerPixelPos.getX());

			// Horizontal
			const auto pointerRSSI = _RSSIMin + (bounds.getHeight() - pointerPixelPos.getY()) * (_RSSIMax - _RSSIMin) / bounds.getHeight();
			auto text = std::format(L"{} dBm", pointerRSSI);
			auto textWidth = Theme::fontSmall.getWidth(text);

			renderer->renderString(
				Point(
					bounds.getX() + textHOffset,
					bounds.getY() + pointerPixelPos.getY() - Theme::fontSmall.getHeight() / 2
				),
				&Theme::fontSmall,
				&Theme::fg4,
				text
			);

			renderer->renderHorizontalLine(
				Point(
					bounds.getX() + textHOffset + textWidth + textHOffset,
					bounds.getY() + pointerPixelPos.getY()
				),
				bounds.getWidth() - (textHOffset + textWidth + textHOffset),
				&Theme::fg4
			);

			// Vertical
			text = std::format(L"{} MHz", frequency / 1'000'000);
			textWidth = Theme::fontSmall.getWidth(text);

			renderer->renderString(
				Point(
					bounds.getX() + pointerPixelPos.getX() - textWidth / 2,
					bounds.getY() + textVOffset
				),
				&Theme::fontSmall,
				&Theme::fg4,
				text
			);

			renderer->renderVerticalLine(
				Point(
					bounds.getX() + pointerPixelPos.getX(),
					bounds.getY() + textVOffset + Theme::fontSmall.getHeight() + textVOffset
				),
				bounds.getHeight() - (textVOffset + Theme::fontSmall.getHeight() + textVOffset),
				&Theme::fg4
			);

			// Tip
			const auto historyIndex = getHistoryIndex(pointerPixelPos.getX());
			const auto historyRSSI = getRSSI(historyIndex);

			if (historyRSSI > _RSSIMin) {
				const auto historyRSSIHeight = getRSSIHeight(historyRSSI);

				const auto tipY = bounds.getY2() - historyRSSIHeight + 1;

				renderer->renderFilledCircle(
					Point(
						bounds.getX() + pointerPixelPos.getX(),
						tipY
					),
					2,
					&Theme::fg1
				);

				text = std::format(L"{} dBm", historyRSSI);

				renderer->renderString(
					Point(
						bounds.getX() + pointerPixelPos.getX() + 2 + textHOffset,
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
		const auto pointerPixelPos = pointerEventPos - bounds.getTopLeft();

		_pointerPos.setX(std::clamp<float>(pointerPixelPos.getX() / bounds.getWidth(), 0, 1));
		_pointerPos.setY(std::clamp<float>(1.f - pointerPixelPos.getY() / bounds.getHeight(), 0, 1));
	}

	SpectrumScanPage::SpectrumScanPage() {
		// Page title
		title.setText(L"Spectrum scanning");

		// Char
		// chart.setHeight(170);
		rows += &chart;

		// -------------------------------- Frequency --------------------------------

		frequencyRow.setOrientation(Orientation::horizontal);
		frequencyRow.setGap(5);
		rows += &frequencyRow;
		rows.setAutoSize(&frequencyRow);

		// From
		Theme::apply(&frequencyFromTextField);
		frequencyFromTextField.setKeyboardLayoutOptions(KeyboardLayoutOptions::numeric);
		frequencyFromTextField.setText(std::to_wstring(RC::getInstance().getSettings().transceiver.spectrumScanning.frequency.from / 1'000'000));
		frequencyRow += &frequencyFromTitle;

		// To
		Theme::apply(&frequencyToTextField);
		frequencyToTextField.setKeyboardLayoutOptions(KeyboardLayoutOptions::numeric);
		frequencyToTextField.setText(std::to_wstring(RC::getInstance().getSettings().transceiver.spectrumScanning.frequency.to / 1'000'000));
		frequencyRow += &frequencyToTitle;

		// Step
		Theme::apply(&frequencyStepTextField);
		frequencyStepTextField.setKeyboardLayoutOptions(KeyboardLayoutOptions::numeric);
		frequencyStepTextField.setText(std::to_wstring(RC::getInstance().getSettings().transceiver.spectrumScanning.frequency.step / 1'000));
		frequencyRow += &frequencyStepTitle;

		// Presets button
		Theme::applySecondary(&frequencyPresetsButton);
		frequencyPresetsButton.setDefaultBackgroundColor(&Theme::bg2);
		frequencyPresetsButton.setMargin(Margin(0, Theme::fontNormal.getHeight() + frequencyFromTitle.getGap(), 0, 0));
		frequencyPresetsButton.setWidth(24);
		frequencyPresetsButton.setText(L"...");

		frequencyPresetsButton.setOnClick([this] {
			(
				new SpectrumScanningFrequencyPresetsDialog([this](const std::wstring_view from, const std::wstring_view to) {
					frequencyFromTextField.setText(from);
					frequencyToTextField.setText(to);
				})
			)->show();
		});

		frequencyRow.setAutoSize(&frequencyPresetsButton);
		frequencyRow += &frequencyPresetsButton;

		// -------------------------------- Begin button --------------------------------

		Theme::applyPrimary(&confirmButton);
		confirmButton.setText(L"Begin");

		confirmButton.setOnClick([this] {
			auto& rc = RC::getInstance();

			if (rc.getRemoteData().transceiver.spectrumScanning.state == RemoteDataRadioSpectrumScanningState::stopped) {
				// From
				rc.getSettings().transceiver.spectrumScanning.frequency.from =
					std::clamp<uint32_t>(
						StringUtils::tryParseInt32Or(frequencyFromTextField.getText(), 902),
						0,
						1000
					)
					* 1'000'000;

				// To
				rc.getSettings().transceiver.spectrumScanning.frequency.to =
					std::clamp<uint32_t>(
						StringUtils::tryParseInt32Or(frequencyToTextField.getText(), 928),
						0,
						1000
					)
					* 1'000'000;

				// From & to
				if (rc.getSettings().transceiver.spectrumScanning.frequency.from > rc.getSettings().transceiver.spectrumScanning.frequency.to)
					std::swap(rc.getSettings().transceiver.spectrumScanning.frequency.from, rc.getSettings().transceiver.spectrumScanning.frequency.to);

				// Step
				rc.getSettings().transceiver.spectrumScanning.frequency.step =
					std::clamp<uint32_t>(
						StringUtils::tryParseInt32Or(frequencyStepTextField.getText(), 50),
						0,
						1000
					)
					* 1'000;

				rc.getSettings().transceiver.scheduleWrite();

				rc.getRemoteData().transceiver.spectrumScanning.frequency = rc.getSettings().transceiver.spectrumScanning.frequency.from;
				rc.getRemoteData().transceiver.spectrumScanning.state = RemoteDataRadioSpectrumScanningState::startRequested;
			}
			else {
				rc.getRemoteData().transceiver.spectrumScanning.state = RemoteDataRadioSpectrumScanningState::stopRequested;
			}

			updateConfirmButtonText();
		});

		rows += &confirmButton;
		rows.setAutoSize(&confirmButton);

		// -------------------------------- Initialization --------------------------------

		updateConfirmButtonText();
	}

	void SpectrumScanPage::onTick() {
		Layout::onTick();

		updateConfirmButtonText();
	}

	void SpectrumScanPage::updateConfirmButtonText() {
		confirmButton.setText(RC::getInstance().getRemoteData().transceiver.spectrumScanning.state == RemoteDataRadioSpectrumScanningState::stopped ? L"Begin" : L"Stop");
	}
}

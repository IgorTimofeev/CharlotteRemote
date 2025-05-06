#pragma once

#include <esp_log.h>
#include <YOBA/main.h>
#include <YOBA/UI.h>
#include "../../theme.h"
#include "../../../hardware/encoder.h"

namespace pizda {
	using namespace YOBA;

	class RC;

	template<uint8_t digitCount, uint32_t minimum, uint32_t maximum, bool cycling, uint16_t smallChange, uint16_t bigChange>
	class RotaryControl : public Layout {
		public:
			RotaryControl(const std::wstring_view& text) {
				// Button
				button.setVerticalAlignment(Alignment::end);
				button.setHeight(12);
				button.setCornerRadius(2);
				button.setContentMargin(Margin(0, 3, 0, 0));

				button.setCheckMode(ButtonCheckMode::toggle);

				button.setDefaultBackgroundColor(&Theme::bg3);
				button.setPressedBackgroundColor(&Theme::fg1);

				button.setDefaultTextColor(&Theme::fg5);
				button.setPressedTextColor(&Theme::bg1);

				button.setFont(&Theme::fontSmall);
				button.setText(text);

				*this += &button;

				// Background rect
				backgroundRectangle.setFillColor(&Theme::bg2);
				backgroundRectangle.setCornerRadius(button.getCornerRadius());
				backgroundAndSevenLayout += &backgroundRectangle;

				// Seven segment
				seven.setVerticalAlignment(Alignment::center);
				seven.setMargin(Margin(6));
				seven.setDigitCount(digitCount);
				seven.setSegmentThickness(1);
				seven.setSegmentLength(6);
				seven.setDigitCount(digitCount);
				seven.setSecondaryColor(&Theme::fg1);
				seven.setPrimaryColor(&Theme::bg5);
				backgroundAndSevenLayout += &seven;

				backgroundAndSevenLayout.setMargin(Margin(0, 0, 0, button.getSize().getHeight() - button.getCornerRadius() * 2));
				*this += &backgroundAndSevenLayout;

				updateColors();
			}

			Layout backgroundAndSevenLayout {};

			Rectangle backgroundRectangle {};

			SevenSegment seven {};
			Button button {};

			Callback<> rotated {};
			Callback<> switched {};

			void onFocusChanged() override {
				Element::onFocusChanged();

				updateColors();
			}

			void updateColors() {
				backgroundRectangle.setBorderColor(isFocused() ? &Theme::fg1 : nullptr);

//				button.setDefaultBorderColor(isFocused() ? &Theme::fg1 : nullptr);
			}

			void onEvent(Event* event) override {
				Layout::onEvent(event);

				if (event->getTypeID() == TouchDownEvent::typeID) {
					setFocused(true);
				}
				else if (event->getTypeID() == EncoderRotateEvent::typeID) {
					if (!isFocused())
						return;

					const auto rotateEvent = (EncoderRotateEvent*) event;
					const auto rps = rotateEvent->getRPS();
					const int32_t change = (std::abs(rps) < 70 ? smallChange : bigChange) * (rps >= 0 ? 1 : -1);

					int64_t newValue = (int64_t) seven.getValue() + change;

					if (cycling) {
						if (newValue > (int64_t) maximum) {
							newValue = (int64_t) minimum;
						}
						else if (newValue < (int64_t) minimum) {
							newValue = (int64_t) maximum;
						}

						seven.setValue(newValue);
					}
					else {
						seven.setValue(std::clamp(newValue, (int64_t) minimum, (int64_t) maximum));
					}

					rotated();

					event->setHandled(true);
				}
				else if (event->getTypeID() == EncoderPushEvent::typeID) {
					if (!isFocused())
						return;

					const auto pushEvent = (EncoderPushEvent*) event;

					if (!pushEvent->isDown())
						return;

					button.setChecked(!button.isChecked());

					event->setHandled(true);
				}
			}
	};
}
#pragma once

#include <esp_log.h>
#include <YOBA/main.h>
#include <YOBA/UI.h>
#include <UI/theme.h>
#include <YOBA/hardware/rotaryEncoder.h>

namespace pizda {
	using namespace YOBA;

	class RC;

	template<uint8_t digitCount, uint32_t minimum, uint32_t maximum, bool cycling, uint16_t smallChange, uint16_t bigChange>
	class RotaryControl : public Layout {
		public:
			explicit RotaryControl(std::wstring_view text) {
				// Button
				button.setVerticalAlignment(Alignment::end);
				button.setHeight(12);
				button.setCornerRadius(2);
				button.setContentMargin(Margin(0, 3, 0, 0));

				button.setDefaultBackgroundColor(&Theme::bg3);
				button.setActiveBackgroundColor(&Theme::fg1);

				button.setDefaultTextColor(&Theme::fg5);
				button.setActiveTextColor(&Theme::bg1);

				button.setFont(&Theme::fontSmall);
				button.setText(text);

				button.setToggle(true);

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
				seven.setInactiveColor(&Theme::bg5);
				seven.setActiveColor(&Theme::fg1);
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

			void onTouchDown(TouchDownEvent* event) override {
				setFocused(true);
			}

			void onEvent(Event* event) override {
				Layout::onEvent(event);

				if (event->getTypeID() == RotaryEncoderRotationEvent::typeID) {
					if (!isFocused())
						return;

					const auto rotateEvent = static_cast<RotaryEncoderRotationEvent*>(event);
					const auto rps = rotateEvent->getRPS();
					const int32_t change = (std::abs(rps) < 70 ? smallChange : bigChange) * (rps >= 0 ? 1 : -1);

					int64_t newValue = static_cast<int64_t>(seven.getValue()) + change;

					if (cycling) {
						if (newValue > static_cast<int64_t>(maximum)) {
							newValue = static_cast<int64_t>(minimum);
						}
						else if (newValue < static_cast<int64_t>(minimum)) {
							newValue = static_cast<int64_t>(maximum);
						}

						seven.setValue(newValue);
					}
					else {
						seven.setValue(std::clamp(newValue, static_cast<int64_t>(minimum), static_cast<int64_t>(maximum)));
					}

					rotated();

					event->setHandled(true);
				}
				else if (event->getTypeID() == RotaryEncoderSwitchEvent::typeID) {
					if (!isFocused())
						return;

					const auto pushEvent = static_cast<RotaryEncoderSwitchEvent*>(event);

					if (!pushEvent->isPressed())
						return;

					button.setActive(!button.isActive());

					event->setHandled(true);
				}
			}
	};
}
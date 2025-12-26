#pragma once

#include <esp_log.h>
#include <YOBA/main.h>
#include <YOBA/UI.h>
#include <YOBA/hardware/encoder.h>

#include "UI/theme.h"
#include "UI/navigation/MFD/toolbar/instrumentIndicatorLayout.h"
#include "UI/navigation/MFD/toolbar/toolbar.h"

namespace pizda {
	using namespace YOBA;

	class RC;

	template<uint8_t digitCount, uint32_t minimum, uint32_t maximum, bool cycling, uint16_t smallChange, uint16_t bigChange>
	class RotaryControl : public InstrumentIndicatorLayout {
		public:
			explicit RotaryControl(std::wstring_view title, std::wstring_view buttonText) : InstrumentIndicatorLayout(title) {
				// Seven segment
				seven.setAlignment(Alignment::center, Alignment::start);
				seven.setMargin(Margin(0, 6, 0, 0));
				
				seven.setDigitCount(digitCount);
				seven.setDecimalSeparatorSpacing(2);
				
				seven.setSegmentThickness(1);
				seven.setSegmentLength(4);
				
				seven.setInactiveColor(&Theme::bg5);
				seven.setActiveColor(&Theme::fg1);
				
				sevenAndButtonLayout += &seven;
				
				// Button
				button.setVerticalAlignment(Alignment::end);
				button.setHeight(7);
				button.setMargin(Margin(-Toolbar::contentHorizontalMargin, 0, -Toolbar::contentHorizontalMargin, 0));
				
				button.setDefaultBackgroundColor(&Theme::bg3);
				button.setActiveBackgroundColor(&Theme::fg1);
				
				button.setDefaultTextColor(&Theme::fg5);
				button.setActiveTextColor(&Theme::bg1);
				
				button.setFont(&Theme::fontSmall);
				button.setText(buttonText);
				button.setContentMargin(Margin(0, 1, 0, 0));
				
				button.setToggle(true);
				
				sevenAndButtonLayout += &button;
				
				setContent(&sevenAndButtonLayout);
			}

			Layout sevenAndButtonLayout {};

			SevenSegment seven {};
			Button button {};

			Callback<> rotated {};
			Callback<> switched {};
			
			void onEventBeforeChildren(Event* event) override {
				InstrumentIndicatorLayout::onEventBeforeChildren(event);
				
				if (event->getTypeID() == EncoderValueChangedEvent::typeID) {
					if (!isFocused())
						return;

					const auto rotateEvent = static_cast<EncoderValueChangedEvent*>(event);
					const auto rps = rotateEvent->getDPS();
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
				else if (event->getTypeID() == PushButtonEncoderDownEvent::typeID) {
					if (!isFocused())
						return;

					button.setActive(!button.isActive());

					event->setHandled(true);
				}
			}
	};
}
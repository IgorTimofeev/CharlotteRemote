#pragma once

#include <esp_log.h>
#include <YOBA/main.h>
#include <YOBA/UI.h>
#include <YOBA/hardware/encoder.h>

#include "UI/theme.h"
#include "UI/navigation/MFD/toolbar/toolbarSection.h"
#include "UI/navigation/MFD/toolbar/toolbar.h"

namespace pizda {
	using namespace YOBA;

	class RC;

	class RotaryControlButton : public Button {
		public:
			RotaryControlButton() {
				setVerticalAlignment(Alignment::end);
				setHeight(7);
				
				setDefaultBackgroundColor(&Theme::bg3);
				setDefaultTextColor(&Theme::fg5);
				
				setActiveBackgroundColor(&Theme::fg1);
				setActiveTextColor(&Theme::bg1);
				
				setFont(&Theme::fontSmall);
				setContentMargin(Margin(0, 1, 0, 0));
				
				setToggle(true);
			}
		
		protected:
//			void onRender(Renderer* renderer, const Bounds& bounds) override {
//				Button::onRender(renderer, bounds);
//
//				if (isActive()) {
//					renderer->renderHorizontalLine(bounds.getBottomLeft(), bounds.getWidth(), &Theme::fg1);
//				}
//			}
	};
	
	template<uint8_t digitCount, uint32_t minimum, uint32_t maximum, bool cycling, uint16_t smallChange, uint16_t bigChange>
	class RotaryControl : public ToolbarSection {
		public:
			explicit RotaryControl(std::wstring_view title, std::wstring_view buttonText) : ToolbarSection(title) {
				// Seven segment
				seven.setAlignment(Alignment::center, Alignment::start);
				
				seven.setDigitCount(digitCount);
				seven.setDecimalSeparatorSpacing(2);
				
				seven.setSegmentThickness(1);
				seven.setSegmentLength(4);
				
				seven.setInactiveColor(&Theme::bg5);
				seven.setActiveColor(&Theme::fg1);
				
				ToolbarSection::setDefaultMargin(&seven, 6);
				
				*this += &seven;
				
				// Button
				button.setText(buttonText);
				
				*this += &button;
			}

			SevenSegment seven {};
			RotaryControlButton button {};

			Callback<> rotated {};
			Callback<> switched {};
			
			void onEventBeforeChildren(Event* event) override {
				ToolbarSection::onEventBeforeChildren(event);
				
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
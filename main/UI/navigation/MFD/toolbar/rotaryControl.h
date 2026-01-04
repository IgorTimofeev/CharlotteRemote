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

	template<uint8_t digitCount, uint32_t minimum, uint32_t maximum, bool cycling, uint16_t smallChange, uint16_t bigChange>
	class RotaryControl : public ToolbarSection {
		public:
			RotaryControl(std::wstring_view title, std::wstring_view buttonText) : ToolbarSection(title), _bottomText(buttonText) {
				// Seven segment
				seven.setAlignment(Alignment::center, Alignment::start);
				
				seven.setDigitCount(digitCount);
				seven.setDecimalSeparatorSpacing(2);
				
				seven.setSegmentThickness(1);
				seven.setSegmentLength(4);
				
				seven.setInactiveColor(&Theme::bg5);
				seven.setActiveColor(&Theme::fg1);
				
				ToolbarSection::setDefaultMargin(&seven, 7);
				
				*this += &seven;
			}

			SevenSegment seven {};

			Callback<> rotated {};
		
		protected:
			void onEventBeforeChildren(Event* event) override {
				if (event->getTypeID() == PointerDownEvent::typeID) {
					if (isFocused()) {
						pressed();
					}
				}
				
				ToolbarSection::onEventBeforeChildren(event);
				
				if (event->getTypeID() == EncoderValueChangedEvent::typeID) {
					if (!isFocused())
						return;

					const auto rotateEvent = static_cast<EncoderValueChangedEvent*>(event);
					const auto rps = rotateEvent->getDPS();
					const int32_t change = (std::abs(rps) < 90 ? smallChange : bigChange) * (rps >= 0 ? 1 : -1);

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
					
					pressed();
					
					event->setHandled(true);
				}
			}
			
			void onRender(Renderer* renderer, const Bounds& bounds) override {
				ToolbarSection::onRender(renderer, bounds);
				
				constexpr static uint8_t bottomSize = 5;
				
				const auto bottomBounds = Bounds(bounds.getX(), bounds.getY2() - bottomSize + 1, bounds.getWidth(), bottomSize);
				
				renderer->renderFilledRectangle(
					bottomBounds,
					isActive() ? &Theme::fg1 : (isFocused() ? &Theme::bg4 : &Theme::bg3)
				);
				
				renderer->renderString(
					Point(
						bottomBounds.getXCenter() - Theme::fontSmall.getWidth(_bottomText) / 2,
						bottomBounds.getY() - 1
					),
					&Theme::fontSmall,
					isActive() ? &Theme::bg1 : &Theme::fg4,
					_bottomText
				);
			}
			
		private:
			std::wstring _bottomText;
	};
}
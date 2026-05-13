#pragma once

#include <algorithm>

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "UI/navigation/MFD/toolbar/rotaryControl.h"

namespace pizda {
	using namespace YOBA;
	
	class SevenRotaryControl : public RotaryControl {
		public:
			SevenRotaryControl(
				const uint8_t digitCount,
				const int32_t minimum,
				const int32_t maximum,
				const bool cycling,
				const uint16_t smallChange,
				const uint16_t bigChange
			) :
				digitCount(digitCount),
				minimum(minimum),
				maximum(maximum),
				cycling(cycling),
				smallChange(smallChange),
				bigChange(bigChange)
			{
				seven.setAlignment(Alignment::center);
				
				seven.setDigitCount(digitCount);
				seven.setDecimalSeparatorSpacing(2);
				
				seven.setSegmentThickness(1);
				seven.setSegmentLength(4);
				
				seven.setInactiveColor(&Theme::bg5);
				seven.setActiveColor(&Theme::fg1);
			}

			SevenSegment seven {};

			const uint8_t digitCount;
			const int32_t minimum;
			const int32_t maximum;
			const bool cycling;
			const uint16_t smallChange;
			const uint16_t bigChange;

		protected:
			void onRotate(const bool clockwise, const bool big) override {
				if (getVariantIndex() == 0) {
					const auto change = static_cast<int32_t>(big ? bigChange : smallChange) * (clockwise ? 1 : -1);
					
					auto newValue = seven.getValue() + change;
					
					if (cycling) {
						if (newValue > maximum) {
							newValue = minimum;
						}
						else if (newValue < minimum) {
							newValue = maximum;
						}
						
						seven.setValue(newValue);
					}
					else {
						seven.setValue(std::clamp(newValue, minimum, maximum));
					}
				}
			}
	};
}
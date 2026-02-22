#pragma once

#include <algorithm>

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "UI/navigation/MFD/toolbar/rotaryControl.h"

namespace pizda {
	using namespace YOBA;
	
	template<uint8_t digitCount, int32_t minimum, int32_t maximum, bool cycling, uint16_t smallChange, uint16_t bigChange>
	class SevenRotaryControl : public RotaryControl {
		public:
			SevenRotaryControl() {
				seven.setAlignment(Alignment::center);
				
				seven.setDigitCount(digitCount);
				seven.setDecimalSeparatorSpacing(2);
				
				seven.setSegmentThickness(1);
				seven.setSegmentLength(4);
				
				seven.setInactiveColor(Theme::bg5);
				seven.setActiveColor(Theme::fg1);
			}

			SevenSegment seven {};
			
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
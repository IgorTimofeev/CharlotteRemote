#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "UI/navigation/MFD/toolbar/sevenRotaryControl.h"

namespace pizda {
	using namespace YOBA;
	
	class SpeedRotaryControl : public SevenRotaryControl<3, 0, 350, false, 1, 10> {
		public:
			SpeedRotaryControl();
		
		protected:
			std::wstring_view variantIndexToTitle(uint8_t index) override;
			bool isVariantEditable(uint8_t index) override;
			void onRotate(bool clockwise, bool big) override;
			void onPress() override;
			void onTick() override;
	};
}
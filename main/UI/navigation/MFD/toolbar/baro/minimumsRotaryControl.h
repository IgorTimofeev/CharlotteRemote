#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "UI/navigation/MFD/toolbar/sevenRotaryControl.h"

namespace pizda {
	using namespace YOBA;
	
	class MinimumsRotaryControl : public SevenRotaryControl<4, 0, 15000, false, 1, 10> {
		public:
			MinimumsRotaryControl();
		
		protected:
			std::wstring_view variantIndexToTitle(uint8_t index) override;
			bool isVariantEditable(uint8_t index) override;
			void onRotate(bool clockwise, bool big) override;
			void onPress() override;
			void onTick() override;
			
		private:
			void updateColor();
	};
}
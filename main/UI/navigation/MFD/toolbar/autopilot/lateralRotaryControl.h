#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "UI/navigation/MFD/toolbar/sevenRotaryControl.h"

namespace pizda {
	using namespace YOBA;

	class LateralRotaryControlLNAV : public RotaryControlRenderable {
		protected:
			void onRender(Renderer* renderer, const Bounds& bounds) override;
	};
	
	class LateralRotaryControl : public SevenRotaryControl<3, 1, 360, true, 1, 10> {
		public:
			LateralRotaryControl();
			
			LateralRotaryControlLNAV LNAV {};
		
		protected:
			std::wstring_view variantIndexToTitle(uint8_t index) override;
			bool isVariantEditable(uint8_t index) override;
			void onTick() override;
			void onPress() override;
			void onRotate(bool clockwise, bool big) override;
	};
}
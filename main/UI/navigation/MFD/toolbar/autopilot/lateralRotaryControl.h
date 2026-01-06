#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "UI/navigation/MFD/toolbar/sevenRotaryControl.h"

namespace pizda {
	using namespace YOBA;

	class LateralRotaryControlLNAV : public RotaryControlRenderable {
		protected:
			void onRender(Renderer* renderer, const Bounds& bounds) override {
				auto text = L"ULLI";
				
				renderer->renderString(
					Point(
						bounds.getXCenter() - Theme::fontNormal.getWidth(text) / 2,
						bounds.getYCenter() - Theme::fontNormal.getHeight()
					),
					&Theme::fontNormal,
					&Theme::magenta,
					text
				);
				
				text = L"N14W31";
				
				renderer->renderString(
					Point(
						bounds.getXCenter() - Theme::fontNormal.getWidth(text) / 2,
						bounds.getYCenter()
					),
					&Theme::fontNormal,
					&Theme::fg1,
					text
				);
			}
	};
	
	class LateralRotaryControl : public SevenRotaryControl<3, 1, 359, true, 1, 10> {
		public:
			LateralRotaryControl();
			
			LateralRotaryControlLNAV LNAV {};
		
		protected:
			std::wstring_view variantIndexToTitle(uint8_t index) override;
			bool isVariantEditable(uint8_t index) override;
			void onTick() override;
			void onLongPress() override;
			void onRotate(bool clockwise, bool big) override;
	};
}
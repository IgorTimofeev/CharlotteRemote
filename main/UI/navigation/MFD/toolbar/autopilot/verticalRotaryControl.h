#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "UI/navigation/MFD/toolbar/sevenRotaryControl.h"

namespace pizda {
	using namespace YOBA;

	class VerticalRotaryControlALT : public RotaryControlRenderable {
		protected:
			void onRender(Renderer* renderer, const Bounds& bounds) override {
				auto text = L"1500";
				const auto textWidth = Theme::fontNormal.getWidth(text);

				const auto lineWidth = textWidth;
				constexpr static uint8_t lineOffset = 2;

				const auto y = bounds.getYCenter() - (Theme::fontNormal.getHeight() + lineOffset) / 2;
				
				renderer->renderString(
					Point(
						bounds.getXCenter() - textWidth / 2,
						y
					),
					&Theme::fontNormal,
					&Theme::ocean,
					text
				);
				
				renderer->renderHorizontalLine(
					Point(
						bounds.getXCenter() - lineWidth / 2,
						y + Theme::fontNormal.getHeight() + lineOffset
					),
					lineWidth,
					&Theme::ocean
				);
			}
	};
	
	class VerticalRotaryControlVNAV : public RotaryControlRenderable {
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
				
				text = L"1200";
				
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
	
	class VerticalRotaryControl : public SevenRotaryControl<4, 0, 35000, false, 10, 100> {
		public:
			VerticalRotaryControl();
			
			VerticalRotaryControlALT ALT {};
			VerticalRotaryControlVNAV VNAV {};
			
		protected:
			std::wstring_view variantIndexToTitle(uint8_t index) override;
			bool isVariantEditable(uint8_t index) override;
			void onLongPress() override;
			void onRotate(bool clockwise, bool big) override;
			void onTick() override;
	};
}
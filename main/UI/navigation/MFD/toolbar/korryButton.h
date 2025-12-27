#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include "UI/theme.h"

#include "UI/navigation/MFD/toolbar/toolbarSection.h"

namespace pizda {
	using namespace YOBA;

	class KorryButton : public Button {
		public:
			explicit KorryButton(std::wstring_view text) {
				setSize(Size(24, 20));
				
				setDefaultBorderColor(&Theme::bg2);
				setDefaultBackgroundColor(&Theme::bg2);
				setDefaultTextColor(&Theme::fg2);

				setActiveBackgroundColor(&Theme::bg2);
				setActiveBorderColor(&Theme::bg4);
				setActiveTextColor(&Theme::fg1);

				setFont(&Theme::fontSmall);
				setText(text);
			}

			void onRender(Renderer* renderer, const Bounds& bounds) override {
				renderer->renderFilledRectangle(bounds, getCornerRadius(), isActive() ? getActiveBackgroundColor() : getDefaultBackgroundColor());
				
				renderer->renderRectangle(bounds, getCornerRadius(), isActive() ? getActiveBorderColor() : getDefaultBorderColor());
				
				renderer->renderString(
					Point(
						bounds.getXCenter() - getFont()->getWidth(getText()) / 2,
						bounds.getYCenter() - getFont()->getHeight() / 2
					),
					getFont(),
					isActive() ? getActiveTextColor() : getDefaultTextColor(),
					getText()
				);

				constexpr static uint8_t lineMarginHorizontal = 7;
				constexpr static uint8_t lineMarginVertical = 4;
				constexpr static uint8_t lineHeight = 2;

				const uint16_t lineWidth = bounds.getWidth() - lineMarginHorizontal * 2;

				renderer->renderFilledRectangle(
					Bounds(
						bounds.getXCenter() - lineWidth / 2,
						bounds.getY2() - lineMarginVertical,
						lineWidth,
						lineHeight
					),
					isActive() ? &Theme::green : &Theme::bg4
				);
			}
	};
}
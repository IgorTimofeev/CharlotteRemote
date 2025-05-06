#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include "../../theme.h"

namespace pizda {
	using namespace YOBA;

	class KorryButton : public Button {
		public:
			KorryButton(const std::wstring_view& text) {
				setCornerRadius(3);

				setDefaultBackgroundColor(&Theme::bg2);
				setDefaultBorderColor(&Theme::bg3);
				setDefaultTextColor(&Theme::fg3);

				setPressedBackgroundColor(&Theme::bg2);
				setPressedBorderColor(&Theme::bg3);
				setPressedTextColor(&Theme::fg1);

				setFont(&Theme::fontSmall);
				setText(text);
			}

			void onRender(Renderer* renderer, const Bounds& bounds) override {
				renderer->renderFilledRectangle(bounds, getCornerRadius(), isChecked() ? getPressedBackgroundColor() : getDefaultBackgroundColor());
//				renderer->renderRectangle(bounds, getCornerRadius(), isChecked() ? getPressedBorderColor() : getDefaultBorderColor());

				renderer->renderString(
					Point(
						bounds.getXCenter() - getFont()->getWidth(getText()) / 2,
						bounds.getYCenter() - getFont()->getHeight() / 2
					),
					getFont(),
					isChecked() ? getPressedTextColor() : getDefaultTextColor(),
					getText()
				);

				constexpr static const uint8_t lineMarginHorizontal = 7;
				constexpr static const uint8_t lineMarginVertical = 4;
				constexpr static const uint8_t lineHeight = 2;

				const uint16_t lineWidth = bounds.getWidth() - lineMarginHorizontal * 2;

				renderer->renderFilledRectangle(
					Bounds(
						bounds.getXCenter() - lineWidth / 2,
						bounds.getY2() - lineMarginVertical,
						lineWidth,
						lineHeight
					),
					isChecked() ? &Theme::green : &Theme::bg3
				);
			}
	};
}
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
	
	class KorryButton2 : public Button {
		public:
			explicit KorryButton2(std::wstring_view text, const Image* image) : _image(image) {
				setFont(&Theme::fontSmall);
				setText(text);
			}
		
		protected:
			Size onMeasure(const Size& availableSize) override {
				return Size(
					std::max(
						_image->getSize().getWidth(),
						Theme::fontSmall.getWidth(getText())
					) + Toolbar::contentHorizontalMargin * 2,
					
					Theme::fontSmall.getHeight()
					+ _image->getSize().getHeight()
				);
			}
		
			void onRender(Renderer* renderer, const Bounds& bounds) override {
				renderer->renderFilledRectangle(
					Bounds(bounds.getX(), bounds.getY(), bounds.getWidth(), Toolbar::topPanelHeight),
					isActive() ? &Theme::bg3 : &Theme::bg2
				);
				
				if (isActive()) {
					renderer->renderFilledRectangle(
						Bounds(bounds.getX(), bounds.getY() + Toolbar::topPanelHeight, bounds.getWidth(), bounds.getHeight() - Toolbar::topPanelHeight),
						&Theme::bg2
					);
				}
				
				renderer->renderString(
					Point(
						bounds.getXCenter() - getFont()->getWidth(getText()) / 2,
						bounds.getY()
					),
					getFont(),
					isActive() ? &Theme::fg1 : &Theme::fg4,
					getText()
				);
				
				renderer->renderImage(
					Point(
						bounds.getXCenter() - _image->getSize().getWidth() / 2,
						bounds.getYCenter() - _image->getSize().getHeight() / 2
					),
					_image
				);
				
				renderer->renderHorizontalLine(
					Point(
						bounds.getX(),
						bounds.getY2()
					),
					bounds.getWidth(),
					&Theme::fg1
				);
			}
			
		private:
			const Image* _image;
	};
}
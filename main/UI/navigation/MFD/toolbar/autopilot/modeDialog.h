#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "UI/elements/dialogs/dialog.h"
#include "UI/theme.h"

#include <functional>

#include "UI/elements/autopilotValueModeElement.h"
#include "types.h"

namespace pizda {
	using namespace YOBA;

	class KorryButton : public Control, public TextElement, public AutopilotValueModeElement {
		public:
			KorryButton(std::wstring_view text) {
				setSize(Size(36, 36));
				setText(text);
			}
			
			Callback<> click {};
			
		protected:
			void onEvent(Event* event) override {
				Control::onEvent(event);
				
				if (event->getTypeID() == PointerDownEvent::typeID) {
					if (getMode() == AutopilotValueMode::none) {
						setMode(AutopilotValueMode::selected);
					}
					else {
						setMode(AutopilotValueMode::none);
					}
					
					click();
				}
			}
			
			void onRender(Renderer* renderer, const Bounds& bounds) override {
				constexpr static uint8_t cornerRadius = 3;
				
				// Background
				renderer->renderFilledRectangle(
					bounds,
					cornerRadius,
					getMode() == AutopilotValueMode::none ? &Theme::bg3 : &Theme::bg4
				);
				
				// Border
				renderer->renderRectangle(
					bounds,
					cornerRadius,
					getMode() == AutopilotValueMode::none ? &Theme::bg4 : &Theme::bg5
				);
				
				// Text
				renderer->renderString(
					Point(
						bounds.getXCenter() - Theme::fontSmall.getWidth(getText()) / 2,
						bounds.getYCenter() - Theme::fontSmall.getHeight() / 2
					),
					&Theme::fontSmall,
					getMode() == AutopilotValueMode::none ? &Theme::fg2 : &Theme::fg1,
					getText()
				);
				
				// Line
				constexpr static uint8_t lineHorizontalOffset = 10;
				constexpr static uint8_t lineVerticalOffset = 6;
				constexpr static uint8_t lineHeight = 2;
				
				const auto lineWidth = bounds.getWidth() - lineHorizontalOffset * 2;
				
				renderer->renderFilledRectangle(
					Bounds(
						bounds.getXCenter() - lineWidth / 2,
						bounds.getY2() - lineVerticalOffset,
						lineWidth,
						lineHeight
					),
					getMode() == AutopilotValueMode::none ? &Theme::green2 : (getMode() == AutopilotValueMode::selected ? &Theme::yellow : &Theme::green1)
				);
			}
	};
	
	class ModeDialog : public Dialog {
		public:
			ModeDialog();
			
			WrapLayout wrapLayout {};
			
		protected:
			void onTick() override;
		
		private:
			KorryButton ATButton { L"A/T" };
			
			KorryButton HDGButton { L"HDG" };
			
			KorryButton FLCButton { L"FLC" };
			KorryButton ALTButton { L"ALT" };
	};
}
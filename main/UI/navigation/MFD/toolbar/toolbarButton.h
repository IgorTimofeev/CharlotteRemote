#pragma once

#include <functional>

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "UI/theme.h"
#include "UI/navigation/MFD/toolbar/toolbarSection.h"

namespace pizda {
	using namespace YOBA;
	
	class ToolbarButton : public ToolbarSection {
		public:
			ToolbarButton(std::wstring_view title) : ToolbarSection(title) {
			
			}
			
	};
	
	class ImageToolbarButton : public ToolbarSection, public ActiveElement {
		public:
			ImageToolbarButton(const std::wstring_view title, const Image* image) : ToolbarSection(title) {
				_imageView.setMargin(Margin(Toolbar::contentHorizontalMargin, 2, Toolbar::contentHorizontalMargin, 0));
				_imageView.setAlignment(Alignment::center);
				_imageView.setImage(image);
				*this += _imageView;
			}

			std::function<void()> onPressed = nullptr;

		protected:
			void onEventBeforeChildren(Event* event) override {
				if (event->getTypeID() == PointerDownEvent::typeID || event->getTypeID() == PushButtonEncoderDownEvent::typeID) {
					if (isFocused()) {
						if (onPressed)
							onPressed();
						
						event->setHandled(true);
					}
				}
				
				ToolbarSection::onEventBeforeChildren(event);
			}
			
			void onIsActiveChanged() override {
				ActiveElement::onIsActiveChanged();
				
				setBorderColor(isActive() ? &Theme::fg1 : nullptr);
			}
		
		private:
			ImageView _imageView {};
	};
}
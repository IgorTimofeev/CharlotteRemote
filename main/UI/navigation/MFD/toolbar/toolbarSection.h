#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "UI/theme.h"

#include "toolbar.h"

namespace pizda {
	using namespace YOBA;

	class ToolbarSection : public Layout, public ActiveElement {
		public:
			ToolbarSection(std::wstring_view title) {
				_title.setHorizontalAlignment(Alignment::center);
				_title.setMargin(Margin(Toolbar::contentHorizontalMargin, 0, Toolbar::contentHorizontalMargin, 0));
				_title.setFont(&Theme::fontSmall);
				_title.setText(title);
				*this += &_title;
				
				updateColors();
			}
			
			Callback<> isActiveChanged {};
			
			ToolbarSection(std::wstring_view title, Element* element) : ToolbarSection(title) {
				*this += element;
			}
			
			static void setDefaultMargin(Element* element, uint8_t topOffset = 0) {
				element->setMargin(Margin(Toolbar::contentHorizontalMargin, Toolbar::topPanelHeight + topOffset, Toolbar::contentHorizontalMargin, 0));
			}
			
			bool isButtonMode() const {
				return _buttonMode;
			}
			
			void setButtonMode(bool buttonMode) {
				_buttonMode = buttonMode;
			}
		
		protected:
			void onEventBeforeChildren(Event* event) override {
				if (event->getTypeID() != PointerDownEvent::typeID)
					return;
				
				setFocused(true);
				
				if (_buttonMode) {
					setActive(!isActive());
				}
				else {
				
				}
			}
			
			void onFocusChanged() override {
				Element::onFocusChanged();
				
				if (_buttonMode) {
				
				}
				else {
					setActive(isFocused());
				}
			}
			
			void onIsActiveChanged() override {
				ActiveElement::onIsActiveChanged();
				
				updateColors();
				
				isActiveChanged();
			}
			
			void onRender(Renderer* renderer, const Bounds& bounds) override {
				if (isActive()) {
					// Panel
					renderer->renderFilledRectangle(
						Bounds(bounds.getX(), bounds.getY(), bounds.getWidth(), Toolbar::topPanelHeight),
						&Theme::bg4
					);

					// Background
					renderer->renderFilledRectangle(
						Bounds(bounds.getX(), bounds.getY() + Toolbar::topPanelHeight, bounds.getWidth(), bounds.getHeight() - Toolbar::topPanelHeight),
						&Theme::bg3
					);
				}
				
				Layout::onRender(renderer, bounds);
			}

		private:
			bool _buttonMode = false;
			
			Text _title {};
			
			void updateColors() {
				_title.setTextColor(isActive() ? &Theme::fg1 : &Theme::fg5);
			}
	};
}
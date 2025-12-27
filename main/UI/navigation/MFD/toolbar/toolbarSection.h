#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "UI/theme.h"

#include "toolbar.h"

namespace pizda {
	using namespace YOBA;

	class ToolbarSection : public Layout, public TextElement, public ActiveElement {
		public:
			ToolbarSection(std::wstring_view title) {
				setText(title);
			}
			
			ToolbarSection(std::wstring_view title, Element* element) : ToolbarSection(title) {
				*this += element;
			}
			
			static void setDefaultMargin(Element* element, uint8_t topOffset = 0) {
				element->setMargin(Margin(Toolbar::contentHorizontalMargin, Toolbar::topPanelHeight + topOffset, Toolbar::contentHorizontalMargin, 0));
			}
		
		protected:
			void onEventBeforeChildren(Event* event) override {
				if (event->getTypeID() != PointerDownEvent::typeID)
					return;
				
				setFocused(true);
			}

			void onRender(Renderer* renderer, const Bounds& bounds) override {
				if (isFocused()) {
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
				
				// Title
				renderer->renderString(
					Point(bounds.getXCenter() - Theme::fontSmall.getWidth(getText()) / 2, bounds.getY()),
					&Theme::fontSmall,
					isFocused() ? &Theme::fg2 : &Theme::fg3,
					getText()
				);

				Layout::onRender(renderer, bounds);
			}

		private:
	};
}
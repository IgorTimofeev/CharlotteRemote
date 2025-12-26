#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "UI/theme.h"

#include "toolbar.h"

namespace pizda {
	using namespace YOBA;

	class InstrumentIndicatorLayout : public Layout {
		public:
			explicit InstrumentIndicatorLayout(std::wstring_view title) {
				_title.setMargin(Margin(
					Toolbar::contentHorizontalMargin,
					Toolbar::titleVerticalOffset,
					Toolbar::contentHorizontalMargin,
					0
				));

				_title.setFont(&Theme::fontSmall);
				_title.setText(title);
				
				updateTitleColor();

				*this += &_title;
				
				_contentHolder.setMargin(Margin(
					Toolbar::contentHorizontalMargin,
					Toolbar::topPanelHeight,
					Toolbar::contentHorizontalMargin,
					0
				));
				
				*this += &_contentHolder;
			}

			InstrumentIndicatorLayout(std::wstring_view text, Element* content) : InstrumentIndicatorLayout(text) {
				setContent(content);
			}

			void setContent(Element* element) {
				_contentHolder += element;
			}

		protected:
			void onEventBeforeChildren(Event* event) override {
				if (event->getTypeID() == PointerDownEvent::typeID) {
					setFocused(true);
				}
			}

			void onFocusChanged() override {
				Element::onFocusChanged();
				
				updateTitleColor();
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

				Layout::onRender(renderer, bounds);
			}

		private:
			Text _title {};
			Layout _contentHolder {};

			void updateTitleColor() {
				_title.setTextColor(isFocused() ? &Theme::fg2 : &Theme::fg3);
			}
	};
}
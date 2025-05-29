#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include <UI/theme.h>

#include "toolbar.h"

namespace pizda {
	using namespace YOBA;

	class InstrumentIndicatorLayout : public Layout {
		public:
			explicit InstrumentIndicatorLayout(std::wstring_view text) {
				_text.setMargin(Margin(
					Toolbar::contentHorizontalMargin,
					Toolbar::titleVerticalOffset,
					Toolbar::contentHorizontalMargin,
					0
				));

				_text.setFont(&Theme::fontSmall);
				_text.setText(text);

				updateTextColor();

				*this += &_text;
			}

			InstrumentIndicatorLayout(std::wstring_view text, Element* content, bool addHorizontalMargin) : InstrumentIndicatorLayout(text) {
				setContent(content, addHorizontalMargin);
			}

			void setContent(Element* element, bool addHorizontalMargin) {
				element->setMargin(
					addHorizontalMargin
					? Margin(
						Toolbar::contentHorizontalMargin,
						Toolbar::contentPanelMarginTop,
						Toolbar::contentHorizontalMargin,
						Toolbar::contentVerticalMargin
					)
					: Margin(
						0,
						Toolbar::contentPanelMarginTop,
						0,
						Toolbar::contentVerticalMargin
					)
				);

				*this += element;
			}

		protected:
			void onEventBeforeChildren(Event* event) override {
				if (event->getTypeID() == PointerDownEvent::typeID) {
					setFocused(true);
				}
			}

			void onFocusChanged() override {
				Element::onFocusChanged();

				updateTextColor();
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
			Text _text {};

			void updateTextColor() {
				_text.setTextColor(isFocused() ? &Theme::fg2 : &Theme::fg3);
			}
	};
}
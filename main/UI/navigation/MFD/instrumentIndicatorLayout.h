#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "../../theme.h"

#include "instrumentsView.h"

namespace pizda {
	using namespace YOBA;

	class InstrumentIndicatorLayout : public Layout {
		public:
			InstrumentIndicatorLayout(const std::wstring_view& text) {
				_text.setMargin(Margin(
					InstrumentsView::contentHorizontalMargin,
					InstrumentsView::titleVerticalOffset,
					InstrumentsView::contentHorizontalMargin,
					0
				));

				_text.setFont(&Theme::fontSmall);
				_text.setText(text);

				updateTextColor();

				*this += &_text;
			}

			InstrumentIndicatorLayout(const std::wstring_view& text, Element* content, bool addHorizontalMargin) : InstrumentIndicatorLayout(text) {
				setContent(content, addHorizontalMargin);
			}

			void setContent(Element* element, bool addHorizontalMargin) {
				element->setMargin(
					addHorizontalMargin
					? Margin(
						InstrumentsView::contentHorizontalMargin,
						InstrumentsView::contentPanelMarginTop,
						InstrumentsView::contentHorizontalMargin,
						InstrumentsView::contentVerticalMargin
					)
					: Margin(
						0,
						InstrumentsView::contentPanelMarginTop,
						0,
						InstrumentsView::contentVerticalMargin
					)
				);

				*this += element;
			}

		protected:
			void onEvent(Event* event) override {
				Layout::onEvent(event);

				if (event->getTypeID() == TouchDownEvent::typeID) {
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
						Bounds(bounds.getX(), bounds.getY(), bounds.getWidth(), InstrumentsView::topPanelHeight),
						&Theme::bg4
					);

					// Background
					renderer->renderFilledRectangle(
						Bounds(bounds.getX(), bounds.getY() + InstrumentsView::topPanelHeight, bounds.getWidth(), bounds.getHeight() - InstrumentsView::topPanelHeight),
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
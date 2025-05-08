#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "../../theme.h"

#include "instrumentToolbar.h"

namespace pizda {
	using namespace YOBA;

	class InstrumentIndicatorLayout : public Layout {
		public:
			InstrumentIndicatorLayout(std::wstring_view text) {
				_text.setMargin(Margin(
					InstrumentToolbar::contentHorizontalMargin,
					InstrumentToolbar::titleVerticalOffset,
					InstrumentToolbar::contentHorizontalMargin,
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
						InstrumentToolbar::contentHorizontalMargin,
						InstrumentToolbar::contentPanelMarginTop,
						InstrumentToolbar::contentHorizontalMargin,
						InstrumentToolbar::contentVerticalMargin
					)
					: Margin(
						0,
						InstrumentToolbar::contentPanelMarginTop,
						0,
						InstrumentToolbar::contentVerticalMargin
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

			void onRender(Renderer* renderer) override {
				if (isFocused()) {
					const auto& bounds = getBounds();

					// Panel
					renderer->renderFilledRectangle(
						Bounds(bounds.getX(), bounds.getY(), bounds.getWidth(), InstrumentToolbar::topPanelHeight),
						&Theme::bg4
					);

					// Background
					renderer->renderFilledRectangle(
						Bounds(bounds.getX(), bounds.getY() + InstrumentToolbar::topPanelHeight, bounds.getWidth(), bounds.getHeight() - InstrumentToolbar::topPanelHeight),
						&Theme::bg3
					);
				}

				Layout::onRender(renderer);
			}

		private:
			Text _text {};

			void updateTextColor() {
				_text.setTextColor(isFocused() ? &Theme::fg2 : &Theme::fg3);
			}
	};
}
#pragma once

#include "Arduino.h"

#include "../../../lib/yoba/src/main.h"
#include "../../../lib/yoba/src/ui.h"

#include "../theme.h"

namespace pizdanc {
	using namespace yoba;
	using namespace yoba::ui;

	class TaxiDots : public Shape {
		protected:
			void onRender(Renderer* renderer) override {
				Element::onRender(renderer);

				const auto& bounds = getBounds();
				const uint8_t dotSize = bounds.getHeight() / 2;
				const auto color = getPrimaryColor();

				auto step = false;

				for (int32_t x = bounds.getX(); x < bounds.getX2(); x += dotSize) {
					renderer->renderFilledRectangle(
						Bounds(
							x,
							step ? bounds.getY() + dotSize : bounds.getY(),
							dotSize,
							dotSize
						),
						color
					);

					step = !step;
				}
			}
	};

	class MenuItem : public SelectorItem, public TextElement {
		public:
			explicit MenuItem(const std::wstring_view& text) {
				// Selection
				_selectionBackground.setPrimaryColor(&Theme::bg3);
				*this += &_selectionBackground;

				// Text
				_text.setAlignment(Alignment::center);
				_text.setMargin(Margin(8, 0, 8, 2));
				_text.setFont(&Theme::fontNormal);
				_text.setText(text);
				*this += &_text;

				// Line
				_dots.setSize(Size(12, 2));
				_dots.setMargin(Margin(0, 0, 0, 1));
				_dots.setAlignment(Alignment::center, Alignment::end);
				_dots.setPrimaryColor(&Theme::green);
				*this += &_dots;

				updateVisualsFromSelection();
			}

			void setSelected(bool value) override {
				SelectorItem::setSelected(value);

				updateVisualsFromSelection();
			}

		private:
			Rectangle _selectionBackground = Rectangle();
			TaxiDots _dots = TaxiDots();
			Text _text = Text();

			void updateVisualsFromSelection() {
				_selectionBackground.setVisible(isSelected());
				_text.setPrimaryColor(isSelected() ? &Theme::fg1 : &Theme::fg4);
				_dots.setVisible(isSelected());
			}
	};
}
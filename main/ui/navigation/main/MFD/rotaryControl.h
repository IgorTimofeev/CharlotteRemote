#pragma once

#include "src/main.h"
#include "src/ui.h"
#include "../../../theme.h"
#include "../../../../hardware/encoder.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class RotaryControl : public Layout, public FocusableElement {
		public:
			RotaryControl(uint8_t digitCount, uint32_t minimum, uint32_t maximum, uint16_t smallChange, uint16_t bigChange) {

			}

			Callback<bool, bool> rotated {};

			Rectangle backgroundRectangle {};
			SevenSegment seven {};

			uint32_t minimum;
			uint32_t maximum;
			uint32_t smallChange;
			uint32_t bigChange;

		protected:
			void onFocusChanged() override {
				Element::onFocusChanged();

				backgroundRectangle.setBorderColor(isFocused() ? &Theme::accent1 : nullptr);
			}

			void onEvent(Event* event) override {
				Layout::onEvent(event);

				if (event->getTypeID() == TouchDownEvent::typeID) {
					setFocused(true);
				}
				else if (event->getTypeID() == EncoderRotateEvent::typeID) {
					if (!isFocused())
						return;

					auto rotateEvent = (EncoderRotateEvent*) event;
					auto rps = rotateEvent->getRPS();
					rotated(rps > 0, abs(rps) > 60);
				}
			}
	};

	class SingleRotaryControl : public RotaryControl {
		public:
			SingleRotaryControl(uint8_t digitCount) : RotaryControl(digitCount) {
				seven.setMargin(Margin(5));
				*this += &seven;
			}

			Button button {};
	};

	class LeftButtonRotaryControl : public RotaryControl {
		public:
			LeftButtonRotaryControl(const std::wstring_view& text, uint8_t digitCount) : RotaryControl(digitCount) {
				// Button
				button.setToggle(true);
				button.setWidth(30);
				button.setCornerRadius(2);

				button.setDefaultBackgroundColor(&Theme::bg3);
				button.setPressedBackgroundColor(&Theme::fg1);

				button.setDefaultTextColor(&Theme::fg1);
				button.setPressedTextColor(&Theme::bg2);

				button.setFont(&Theme::fontSmall);
				button.setText(text);

				row += &button;

				// Seven
				seven.setMargin(Margin(5));
				row += &seven;

				row.setOrientation(Orientation::horizontal);
				*this += &row;
			}

			StackLayout row {};

			Button button {};
	};
}
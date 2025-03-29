#pragma once

#include "src/main.h"
#include "src/ui.h"
#include "../../../theme.h"
#include "../../../../hardware/encoder.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	template<uint8_t digitCount, uint32_t minimum, uint32_t maximum, uint16_t smallChange, uint16_t bigChange>
	class RotaryControl : public Layout, public FocusableElement {
		public:
			RotaryControl() {
				setHeight(30);

				// Background
				backgroundRectangle.setCornerRadius(2);
				backgroundRectangle.setFillColor(&Theme::bg2);
				*this += &backgroundRectangle;

				// Seven segment
				seven.setVerticalAlignment(Alignment::center);
				seven.setDigitCount(digitCount);
				seven.setPrimaryColor(&Theme::bg4);
				seven.setSecondaryColor(&Theme::fg1);
				seven.setSegmentThickness(1);
				seven.setSegmentLength(8);
				seven.setDigitCount(digitCount);
			}

			Callback<> rotated {};

			Rectangle backgroundRectangle {};
			SevenSegment seven {};

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

					seven.setValue(std::clamp(seven.getValue() + (std::abs(rps) > 60 ? bigChange : smallChange) * (rps >= 0 ? 1 : -1), minimum, maximum));

					rotated();
				}
			}
	};

	template<uint8_t digitCount, uint32_t minimum, uint32_t maximum, uint16_t smallChange, uint16_t bigChange>
	class SingleRotaryControl : public RotaryControl<digitCount, minimum, maximum, smallChange, bigChange> {
		public:
			SingleRotaryControl() {
				this->seven.setMargin(Margin(5));
				*this += &this->seven;
			}

			Button button {};
	};

	template<uint8_t digitCount, uint32_t minimum, uint32_t maximum, uint16_t smallChange, uint16_t bigChange>
	class LeftButtonRotaryControl : public RotaryControl<digitCount, minimum, maximum, smallChange, bigChange> {
		public:
			LeftButtonRotaryControl(const std::wstring_view& text) {
				// Button
				button.setToggle(true);
				button.setWidth(28);
				button.setCornerRadius(2);

				button.setDefaultBackgroundColor(&Theme::bg3);
				button.setPressedBackgroundColor(&Theme::fg1);

				button.setDefaultTextColor(&Theme::fg1);
				button.setPressedTextColor(&Theme::bg2);

				button.setFont(&Theme::fontSmall);
				button.setText(text);

				row += &button;

				// Seven
				this->seven.setMargin(Margin(5));
				row += &this->seven;

				// Row
				row.setOrientation(Orientation::horizontal);
				*this += &row;
			}

			StackLayout row {};

			Button button {};

			template<typename TValue>
			void addSettingsCallbacks(TValue* numericValue, bool* boolValue);
	};
}
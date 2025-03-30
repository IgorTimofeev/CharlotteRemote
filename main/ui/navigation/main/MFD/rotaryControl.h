#pragma once

#include "src/main.h"
#include "src/ui.h"
#include "../../../theme.h"
#include "../../../../hardware/encoder.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class RC;

	template<uint8_t digitCount, uint32_t minimum, uint32_t maximum, bool cycling, uint16_t smallChange, uint16_t bigChange>
	class RotaryControl : public Layout, public FocusableElement {
		public:
			RotaryControl(const std::wstring_view& text) {
				// Background
				backgroundRectangle.setFillColor(&Theme::bg2);
				backgroundRectangle.setCornerRadius(2);
				*this += &backgroundRectangle;

				// Button
				button.setToggle(true);
				button.setWidth(24);
				button.setCornerRadius(backgroundRectangle.getCornerRadius());

				button.setDefaultBackgroundColor(&Theme::bg3);
				button.setPressedBackgroundColor(&Theme::fg1);

				button.setDefaultTextColor(&Theme::fg1);
				button.setPressedTextColor(&Theme::bg2);

				button.setFont(&Theme::fontNormal);
				button.setText(text);

				button.isCheckedChanged += [this]() {
					switched();
				};

				row += &button;

				// Seven segment
				seven.setVerticalAlignment(Alignment::center);
				seven.setMargin(Margin(7));
				seven.setDigitCount(digitCount);
				seven.setSegmentThickness(1);
				seven.setSegmentLength(5);
				seven.setDigitCount(digitCount);
				seven.setSecondaryColor(&Theme::fg1);
				seven.setPrimaryColor(&Theme::bg4);
				row += &seven;

				// Row
				row.setOrientation(Orientation::horizontal);
				*this += &row;

				updateColors();
			}

			Rectangle backgroundRectangle {};
			StackLayout row {};

			SevenSegment seven {};
			Button button {};

			Callback<> rotated {};
			Callback<> switched {};

			void onFocusChanged() override {
				Element::onFocusChanged();

				updateColors();
			}

			void updateColors() {
				backgroundRectangle.setBorderColor(isFocused() ? &Theme::fg1 : nullptr);

//				button.setDefaultBorderColor(isFocused() ? &Theme::fg1 : nullptr);
			}

			void onEvent(Event* event) override {
				Layout::onEvent(event);

				if (event->getTypeID() == TouchDownEvent::typeID) {
					setFocused(true);
				}
				else if (event->getTypeID() == EncoderRotateEvent::typeID) {
					if (!isFocused())
						return;

					const auto rotateEvent = (EncoderRotateEvent*) event;
					const auto rps = rotateEvent->getRPS();
					const auto change = (std::abs(rps) > 60 ? bigChange : smallChange) * (rps >= 0 ? 1 : -1);

					if (cycling) {
						auto newValue = (int32_t) seven.getValue() + change;

						if (newValue > maximum) {
							newValue = minimum;
						}
						else if (newValue < minimum) {
							newValue = maximum;
						}

						seven.setValue(newValue);
					}
					else {
						seven.setValue(std::clamp((int32_t) seven.getValue() + change, (int32_t) minimum, (int32_t) maximum));
					}

					rotated();

					event->setHandled(true);
				}
				else if (event->getTypeID() == EncoderPushEvent::typeID) {
					if (!isFocused())
						return;

					const auto pushEvent = (EncoderPushEvent*) event;

					if (!pushEvent->isDown())
						return;

					button.setChecked(!button.isChecked());

					event->setHandled(true);
				}
			}
	};
}
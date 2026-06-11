#include "theme.h"

namespace pizda {
	void Theme::setup() {
		// Keyboard
		KeyboardController::setOnShow([](Keyboard* keyboard) {
			keyboard->setFont(&Theme::fontNormal);
			keyboard->setBackgroundColor(&Theme::bg1);

			keyboard->setKeyHeight(24);
			keyboard->setKeyCornerRadius(0);

			keyboard->setHorizontalKeySpacing(1);
			keyboard->setVerticalKeySpacing(1);

			keyboard->setDefaultKeyBackgroundColor(&Theme::bg3);
			keyboard->setDefaultKeyTextColor(&Theme::fg1);

			keyboard->setActionKeyBackgroundColor(&Theme::bg2);
			keyboard->setActionKeyTextColor(&Theme::fg1);

			keyboard->setDisabledKeyBackgroundColor(&Theme::bg2);
			keyboard->setDisabledKeyTextColor(&Theme::fg5);

			keyboard->setNumericLayoutBuilder([] {
				return new NumericKeyboardLayout();
			});

			keyboard->setCharactersLayoutBuilder([] {
				return new CharactersKeyboardLayout();
			});

			keyboard->setCyclicLayoutBuilders({
				[] {
					return new EnglishKeyboardLayout();
				}
			});
		});
	}

	void Theme::apply(TextView* text) {
		text->setTextColor(&Theme::fg1);
		text->setFont(&Theme::fontNormal);
	}

	void Theme::applyDescription(TextView* text) {
		text->setTextColor(&Theme::fg3);
		text->setFont(&Theme::fontNormal);
	}

	void Theme::applyPageTitle(TextView* text) {
		text->setTextColor(&Theme::fg1);
		text->setFont(&Theme::fontNormal);
		text->setFontScale(2);
	}

	void Theme::applyTitle(TextView* text) {
		text->setFont(&Theme::fontNormal);
		text->setTextColor(&Theme::fg3);
	}

	void Theme::applyPrimary(Button* button) {
		button->setHeight(elementHeight);
		button->setCornerRadius(cornerRadius);

		button->setDefaultBackgroundColor(&Theme::accent1);
		button->setDefaultTextColor(&Theme::bg1);

		button->setActiveBackgroundColor(&Theme::accent2);
		button->setActiveTextColor(&Theme::bg1);

		button->setFont(&Theme::fontNormal);
	}

	void Theme::applySecondary(Button* button) {
		button->setHeight(elementHeight);
		button->setCornerRadius(cornerRadius);

		button->setDefaultBackgroundColor(&Theme::bg3);
		button->setDefaultTextColor(&Theme::fg3);

		button->setActiveBackgroundColor(&Theme::bg5);
		button->setActiveTextColor(&Theme::fg1);

		button->setFont(&Theme::fontNormal);
	}

	void Theme::applyCritical(Button* button) {
		button->setHeight(elementHeight);
		button->setCornerRadius(cornerRadius);

		button->setDefaultBackgroundColor(&Theme::bad1);
		button->setDefaultTextColor(&Theme::fg1);

		button->setActiveBackgroundColor(&Theme::bad3);
		button->setActiveTextColor(&Theme::fg1);

		button->setFont(&Theme::fontNormal);
	}

	void Theme::applyPlaceholder(Button* button) {
		button->setHeight(elementHeight);
		button->setCornerRadius(cornerRadius);

		button->setDefaultBorderColor(&Theme::fg5);
		button->setDefaultTextColor(&Theme::fg5);

		button->setActiveBorderColor(&Theme::fg1);
		button->setActiveTextColor(&Theme::fg1);

		button->setFont(&Theme::fontNormal);
	}

	void Theme::apply(Switch* sw) {
		sw->setSize(Size(38, 16));
		sw->setCornerRadius(sw->getSize().getHeight() / 2);

		sw->setTrackColor(&Theme::bg3);
		sw->setCheckedColor(&Theme::accent1);

		sw->setHandleColor(&Theme::fg1);
	}

	void Theme::apply(Slider* slider) {
		slider->setMinimumValue(0);
		slider->setMaximumValue(1);

		// Track
		slider->setTrackSize(5);
		slider->setTrackCornerRadius(slider->getTrackSize() / 2);
		slider->setTrackColor(&Theme::bg3);

		// Fill
		slider->setFillColor(&Theme::accent1);

		// Handle
		slider->setHandleSize({ 2, 10 });
		slider->setHandleCornerRadius(0);
		slider->setHandleColor(&Theme::fg1);

		// Tick
		slider->setTickCount(10);
		slider->setBigTickStep(5);
		slider->setTickOffset(0);
		slider->setBigTickLineLength(5);
		slider->setSmallTickLineLength(3);
		slider->setTickColor(&Theme::fg6);

		slider->setTickLabelFont(&Theme::fontNormal);
		slider->setTickLabelOffset(1);

		slider->setTickLabelBuilder(Slider::percentTickLabelBuilder);
	}

	void Theme::apply(ProgressBar* progressBar) {
		progressBar->setHeight(5);
		progressBar->setCornerRadius(2);
		progressBar->setTrackColor(&Theme::bg3);
		progressBar->setFillColor(&Theme::accent1);
	}

	void Theme::apply(ScrollView* scrollView) {
		scrollView->setScrollBarSize(3);
		scrollView->setScrollBarCornerRadius(1);
		scrollView->setScrollBarOffset(3);
		scrollView->setScrollBarThumbColor(&Theme::bg3);
	}

	void Theme::apply(TextField* textField) {
		textField->setHeight(elementHeight);
		textField->setCornerRadius(cornerRadius);
		textField->setTextMargin(10);

		textField->setDefaultBackgroundColor(&Theme::bg3);
		textField->setFocusedBackgroundColor(&Theme::bg4);

		textField->setDefaultTextColor(&Theme::fg1);
		textField->setFocusedTextColor(&Theme::fg1);

		textField->setDefaultBorderColor(&Theme::bg4);
		textField->setFocusedBorderColor(&Theme::accent1);
		
		textField->setPlaceholderColor(&Theme::fg5);
		textField->setCursorColor(&Theme::accent1);
		
		textField->setFont(&Theme::fontNormal);
	}
}
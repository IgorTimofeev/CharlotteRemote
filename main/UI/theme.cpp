#include "theme.h"

namespace pizda {
	void Theme::setup(Bit8PaletteRenderer* renderer) {
		renderer->setPaletteColors({
			// Background
			RGB888Color(0x000000),
			RGB888Color(0x0C0C0C),
			RGB888Color(0x1C1C1C),
			RGB888Color(0x2C2C2C),
			RGB888Color(0x3C3C3C),
			RGB888Color(0x4C4C4C),
			RGB888Color(0x5C5C5C),

			// Foreground
			RGB888Color(0xF0F0F0),
			RGB888Color(0xCCCCCC),
			RGB888Color(0x999999),
			RGB888Color(0x888888),
			RGB888Color(0x777777),
			RGB888Color(0x666666),
			RGB888Color(0x555555),

			// Accent 1
			RGB888Color(0xffd200),
			// Accent 2
			RGB888Color(0x997e53),

			// Good 1
			RGB888Color(0x1db153),
			// Good 2
			RGB888Color(0x28DA69),

			// Bad 1
			RGB888Color(0x9f2d2d),
			// Bad 2
			RGB888Color(0xf24949),

			// Red
			RGB888Color(0xff0000),
			// Purple
			RGB888Color(0xff00ff),
			// Green
			RGB888Color(0x00FF00),
			// Green speed
			RGB888Color(0x008100),
			// Yellow
			RGB888Color(0xffd200),
			// Ocean
			RGB888Color(0x00ffff),
			// Ground
			RGB888Color(0x97b838),
			// Ground 2
			RGB888Color(0xdef2a2),
			// Sky
			RGB888Color(0x317fcb),
			// Sky 2
			RGB888Color(0xaed1f2),
		});

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

			keyboard->setCharactersLayoutBuilder([] {
				return new CharactersKeyboardLayout();
			});

			keyboard->setCyclicLayoutBuilders({
				[] {
					return new EnglishKeyboardLayout();
				},
//			[]() {
//				return new RussianKeyboardLayout();
//			}
			});

			keyboard->setCyclicLayoutIndex(0);
		});
	}

	void Theme::apply(Text* text) {
		text->setTextColor(&Theme::fg1);
		text->setFont(&Theme::fontNormal);
	}

	void Theme::applyDescription(Text* text) {
		text->setTextColor(&Theme::fg3);
		text->setFont(&Theme::fontNormal);
	}

	void Theme::applyPageTitle(Text* text) {
		text->setTextColor(&Theme::fg1);
		text->setFont(&Theme::fontBig);
//		text->setFontScale(2);
	}

	void Theme::applyTitle(Text* text) {
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

		button->setDefaultBackgroundColor(&Theme::bad2);
		button->setDefaultTextColor(&Theme::fg1);

		button->setActiveBackgroundColor(&Theme::bad1);
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
		sw->setSize(Size(45, 18));
		sw->setCornerRadius(9);

		sw->setTrackColor(&Theme::bg3);
		sw->setCheckedColor(&Theme::accent1);

		sw->setHandleColor(&Theme::fg1);
	}

	void Theme::apply(Slider* slider) {
		slider->setHeight(14);

		slider->setTrackSize(4);
		slider->setTrackCornerRadius(2);
		slider->setTrackColor(&Theme::bg3);
		slider->setFillColor(&Theme::accent1);

		slider->setHandleSize(14);
		slider->setHandleCornerRadius(7);
		slider->setHandleColor(&Theme::fg1);
	}

	void Theme::apply(ProgressBar* progressBar) {
		progressBar->setHeight(4);
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
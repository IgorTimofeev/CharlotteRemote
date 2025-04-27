#include "theme.h"

namespace pizda {
	const PaletteColor Theme::bg1 = PaletteColor(0);
	const PaletteColor Theme::bg2 = PaletteColor(1);
	const PaletteColor Theme::bg3 = PaletteColor(2);
	const PaletteColor Theme::bg4 = PaletteColor(3);
	const PaletteColor Theme::bg5 = PaletteColor(4);
	const PaletteColor Theme::bg6 = PaletteColor(5);
	const PaletteColor Theme::bg7 = PaletteColor(6);

	const PaletteColor Theme::fg1 = PaletteColor(7);
	const PaletteColor Theme::fg2 = PaletteColor(8);
	const PaletteColor Theme::fg3 = PaletteColor(9);
	const PaletteColor Theme::fg4 = PaletteColor(10);
	const PaletteColor Theme::fg5 = PaletteColor(11);
	const PaletteColor Theme::fg6 = PaletteColor(12);
	const PaletteColor Theme::fg7 = PaletteColor(13);

	const PaletteColor Theme::accent1 = PaletteColor(14);
	const PaletteColor Theme::accent2 = PaletteColor(15);

	const PaletteColor Theme::good1 = PaletteColor(16);
	const PaletteColor Theme::good2 = PaletteColor(17);

	const PaletteColor Theme::bad1 = PaletteColor(18);
	const PaletteColor Theme::bad2 = PaletteColor(19);

	const PaletteColor Theme::red = PaletteColor(20);
	const PaletteColor Theme::purple = PaletteColor(21);
	const PaletteColor Theme::green = PaletteColor(22);
	const PaletteColor Theme::greenSpeed = PaletteColor(23);
	const PaletteColor Theme::yellow = PaletteColor(24);
	const PaletteColor Theme::blue = PaletteColor(25);
	const PaletteColor Theme::ground = PaletteColor(26);
	const PaletteColor Theme::ground2 = PaletteColor(27);
	const PaletteColor Theme::sky = PaletteColor(28);
	const PaletteColor Theme::sky2 = PaletteColor(29);

	const PIXY10Font Theme::fontNormal = PIXY10Font();
	const Vaticanus8Font Theme::fontSmall = Vaticanus8Font();

	void Theme::setup(Bit8PaletteRenderer* renderer) {
		renderer->setPaletteColors({
			// Background
			Rgb888Color(0x000000),
			Rgb888Color(0x0C0C0C),
			Rgb888Color(0x1C1C1C),
			Rgb888Color(0x2C2C2C),
			Rgb888Color(0x3C3C3C),
			Rgb888Color(0x4C4C4C),
			Rgb888Color(0x5C5C5C),

			// Foreground
			Rgb888Color(0xF0F0F0),
			Rgb888Color(0xCCCCCC),
			Rgb888Color(0x999999),
			Rgb888Color(0x888888),
			Rgb888Color(0x777777),
			Rgb888Color(0x666666),
			Rgb888Color(0x555555),

			// Accent 1
			Rgb888Color(0xffd200),
			// Accent 2
			Rgb888Color(0x997e53),

			// Good 1
			Rgb888Color(0x1db153),
			// Good 2
			Rgb888Color(0x28DA69),

			// Bad 1
			Rgb888Color(0x9f2d2d),
			// Bad 2
			Rgb888Color(0xf24949),

			// Red
			Rgb888Color(0xff0000),
			// Purple
			Rgb888Color(0xff00ff),
			// Green
			Rgb888Color(0x00FF00),
			// Green speed
			Rgb888Color(0x008100),
			// Yellow
			Rgb888Color(0xffd200),
			// Ocean
			Rgb888Color(0x00ffff),
			// Ground
			Rgb888Color(0x97b838),
			// Ground 2
			Rgb888Color(0xdef2a2),
			// Sky
			Rgb888Color(0x317fcb),
			// Sky 2
			Rgb888Color(0xaed1f2),
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

	void Theme::applyTitle(Text* text) {
		text->setTextColor(&Theme::fg1);
		text->setFont(&Theme::fontNormal);
		text->setFontScale(2);
	}

	void Theme::apply(Button* button) {
		button->setHeight(elementHeight);
		button->setCornerRadius(cornerRadius);

		button->setDefaultBackgroundColor(&Theme::accent1);
		button->setPressedBackgroundColor(&Theme::accent2);

		button->setDefaultTextColor(&Theme::bg1);
		button->setPressedTextColor(&Theme::bg1);

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

		textField->setPrimaryColor(&Theme::bg3);
		textField->setFocusedPrimaryColor(&Theme::bg4);

		textField->setSecondaryColor(&Theme::fg1);
		textField->setFocusedSecondaryColor(&Theme::fg1);

		textField->setBorderColor(&Theme::bg4);
		textField->setFocusedBorderColor(&Theme::accent1);

		textField->setCursorColor(&Theme::accent1);

		textField->setFont(&Theme::fontNormal);

		textField->setKeyboardConfigurator([](Keyboard* keyboard) {
			keyboard->setFont(&Theme::fontNormal);
			keyboard->setBackgroundColor(&Theme::bg2);

			keyboard->setKeyHeight(26);
			keyboard->setKeyCornerRadius(0);

			keyboard->setHorizontalKeySpacing(1);
			keyboard->setVerticalKeySpacing(1);

			keyboard->setDefaultKeyBackgroundColor(&Theme::bg4);
			keyboard->setDefaultKeyTextColor(&Theme::fg1);
			keyboard->setActionKeyBackgroundColor(&Theme::bg3);
			keyboard->setActionKeySecondaryColor(&Theme::fg1);

			keyboard->setCharactersLayoutBuilder([]() {
				return new CharactersKeyboardLayout();
			});

			keyboard->setCyclicLayoutBuilders({
				[]() {
					return new EnglishKeyboardLayout();
				},
//			[]() {
//				return new RussianKeyboardLayout();
//			}
			});

			keyboard->setCyclicLayoutIndex(0);
		});
	}
}
#include "theme.h"

namespace pizda {
	const Rgb565Color Theme::bg1 = Rgb888Color(0x000000).toRgb565();
	const Rgb565Color Theme::bg2 = Rgb888Color(0x0C0C0C).toRgb565();
	const Rgb565Color Theme::bg3 = Rgb888Color(0x1C1C1C).toRgb565();
	const Rgb565Color Theme::bg4 = Rgb888Color(0x2C2C2C).toRgb565();
	const Rgb565Color Theme::bg5 = Rgb888Color(0x3C3C3C).toRgb565();
	const Rgb565Color Theme::bg6 = Rgb888Color(0x4C4C4C).toRgb565();
	const Rgb565Color Theme::bg7 = Rgb888Color(0x5C5C5C).toRgb565();

	const Rgb565Color Theme::fg1 = Rgb888Color(0xF0F0F0).toRgb565();
	const Rgb565Color Theme::fg2 = Rgb888Color(0xCCCCCC).toRgb565();
	const Rgb565Color Theme::fg3 = Rgb888Color(0x999999).toRgb565();
	const Rgb565Color Theme::fg4 = Rgb888Color(0x888888).toRgb565();
	const Rgb565Color Theme::fg5 = Rgb888Color(0x777777).toRgb565();
	const Rgb565Color Theme::fg6 = Rgb888Color(0x666666).toRgb565();
	const Rgb565Color Theme::fg7 = Rgb888Color(0x555555).toRgb565();

	const Rgb565Color Theme::accent1 = Rgb888Color(0xffd200).toRgb565();
	const Rgb565Color Theme::accent2 = Rgb888Color(0x997e53).toRgb565();

	const Rgb565Color Theme::good1 = Rgb888Color(0x1db153).toRgb565();
	const Rgb565Color Theme::good2 = Rgb888Color(0x28DA69).toRgb565();

	const Rgb565Color Theme::bad1 = Rgb888Color(0x9f2d2d).toRgb565();
	const Rgb565Color Theme::bad2 = Rgb888Color(0xf24949).toRgb565();

	const Rgb565Color Theme::red = Rgb888Color(0xff0000).toRgb565();
	const Rgb565Color Theme::purple = Rgb888Color(0xff00ff).toRgb565();
	const Rgb565Color Theme::green = Rgb888Color(0x00FF00).toRgb565();
	const Rgb565Color Theme::greenSpeed = Rgb888Color(0x008100).toRgb565();
	const Rgb565Color Theme::yellow = Rgb888Color(0xffd200).toRgb565();
	const Rgb565Color Theme::blue = Rgb888Color(0x00ffff).toRgb565();
	const Rgb565Color Theme::ground = Rgb888Color(0x97b838).toRgb565();
	const Rgb565Color Theme::ground2 = Rgb888Color(0xdef2a2).toRgb565();
	const Rgb565Color Theme::sky = Rgb888Color(0x317fcb).toRgb565();
	const Rgb565Color Theme::sky2 = Rgb888Color(0xaed1f2).toRgb565();

	const PIXY10Font Theme::fontNormal = PIXY10Font();
	const Vaticanus8Font Theme::fontSmall = Vaticanus8Font();

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
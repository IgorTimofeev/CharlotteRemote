#include "theme.h"

#include "../../lib/yoba/src/main.h"

namespace pizdanc {
	const PaletteColor Theme::bg1 = PaletteColor(0);
	const PaletteColor Theme::bg2 = PaletteColor(1);
	const PaletteColor Theme::bg3 = PaletteColor(2);
	const PaletteColor Theme::bg4 = PaletteColor(3);
	const PaletteColor Theme::bg5 = PaletteColor(4);

	const PaletteColor Theme::fg1 = PaletteColor(5);
	const PaletteColor Theme::fg2 = PaletteColor(6);
	const PaletteColor Theme::fg3 = PaletteColor(7);
	const PaletteColor Theme::fg4 = PaletteColor(8);

	const PaletteColor Theme::red = PaletteColor(9);
	const PaletteColor Theme::purple = PaletteColor(10);
	const PaletteColor Theme::green = PaletteColor(11);
	const PaletteColor Theme::greenSpeed = PaletteColor(12);
	const PaletteColor Theme::yellow = PaletteColor(13);
	const PaletteColor Theme::blue = PaletteColor(14);
	const PaletteColor Theme::ground = PaletteColor(15);
	const PaletteColor Theme::ground2 = PaletteColor(16);
	const PaletteColor Theme::sky = PaletteColor(17);
	const PaletteColor Theme::sky2 = PaletteColor(18);

	const PIXY10Font Theme::fontNormal = PIXY10Font();
	const Vaticanus8Font Theme::fontSmall = Vaticanus8Font();
//	const unscii16Font Theme::fontBig = unscii16Font();

	void Theme::setup(Bit8PaletteBufferedRenderer* renderer) {
		renderer->setPaletteColors({
			// Background
			Rgb888Color(0x000000),
			Rgb888Color(0x0C0C0C),
			Rgb888Color(0x1C1C1C),
			Rgb888Color(0x2C2C2C),
			Rgb888Color(0x3C3C3C),
			// Foreground
			Rgb888Color(0xF0F0F0),
			Rgb888Color(0xDDDDDD),
			Rgb888Color(0x999999),
			Rgb888Color(0x777777),
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
			Rgb888Color(0xaed1f2)
		});
	}

	void Theme::apply(Titler& titler) {
		titler.getTitle().setPrimaryColor(&Theme::fg3);
		titler.getTitle().setFont(&Theme::fontNormal);
	}

	void Theme::apply(Button &button) {
		button.setHeight(elementHeight);
		button.setCornerRadius(cornerRadius);

		button.setPrimaryColor(&Theme::yellow);
		button.setPressedPrimaryColor(&Theme::purple);

		button.setSecondaryColor(&Theme::fg1);
		button.setPressedSecondaryColor(&Theme::fg1);

		button.setFont(&Theme::fontNormal);
	}

	void Theme::apply(Slider &slider) {
		slider.setHeight(elementHeight);
		slider.setCornerRadius(cornerRadius);

		slider.setPrimaryColor(&Theme::bg3);
		slider.setSecondaryColor(&Theme::yellow);
	}

	void Theme::apply(TextField& textField) {
		textField.setHeight(elementHeight);
		textField.setCornerRadius(cornerRadius);

		textField.setPrimaryColor(&Theme::bg3);
		textField.setFocusedPrimaryColor(&Theme::bg4);

		textField.setSecondaryColor(&Theme::fg1);
		textField.setFocusedSecondaryColor(&Theme::fg1);

		textField.setBorderColor(&Theme::bg4);
		textField.setFocusedBorderColor(&Theme::yellow);

		textField.setCursorColor(&Theme::yellow);

		textField.setFont(&Theme::fontNormal);

		textField.setKeyboardConfigurator([](Keyboard* keyboard) {
			keyboard->setKeyHeight(0.08f);
			keyboard->setFont(&Theme::fontNormal);
			keyboard->setBackgroundColor(&Theme::bg2);
			keyboard->setDefaultButtonPrimaryColor(&Theme::bg5);
			keyboard->setDefaultButtonSecondaryColor(&Theme::fg1);
			keyboard->setActionButtonPrimaryColor(&Theme::bg3);
			keyboard->setActionButtonSecondaryColor(&Theme::fg1);

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
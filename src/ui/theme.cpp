#include "theme.h"

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
	const unscii16Font Theme::fontBig = unscii16Font();

	void Theme::apply(Button &button) {
		button.setHeight(elementHeight);
		button.setCornerRadius(cornerRadius);

		button.setPrimaryColor(&Theme::yellow);
		button.setPressedPrimaryColor(&Theme::purple);

		button.setSecondaryColor(&Theme::fg1);
		button.setPressedSecondaryColor(&Theme::fg1);
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
		textField.setSecondaryColor(&Theme::fg1);
		textField.setBorderColor(&Theme::yellow);
		textField.setCursorColor(&Theme::yellow);

		textField.setKeyboardFont(&Theme::fontNormal);
		textField.setKeyboardBackgroundColor(&Theme::bg3);
		textField.setKeyboardDefaultButtonPrimaryColor(&Theme::bg5);
		textField.setKeyboardDefaultButtonSecondaryColor(&Theme::fg1);
		textField.setKeyboardActionButtonPrimaryColor(&Theme::bg4);
		textField.setKeyboardActionButtonSecondaryColor(&Theme::fg1);
	}
}
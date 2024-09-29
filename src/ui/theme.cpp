#include "theme.h"

namespace pizdanc {
	const Bit8PaletteColor Theme::bg1 = Bit8PaletteColor(0);
	const Bit8PaletteColor Theme::bg2 = Bit8PaletteColor(1);
	const Bit8PaletteColor Theme::bg3 = Bit8PaletteColor(2);
	const Bit8PaletteColor Theme::bg4 = Bit8PaletteColor(3);
	const Bit8PaletteColor Theme::bg5 = Bit8PaletteColor(4);

	const Bit8PaletteColor Theme::fg1 = Bit8PaletteColor(5);
	const Bit8PaletteColor Theme::fg2 = Bit8PaletteColor(6);
	const Bit8PaletteColor Theme::fg3 = Bit8PaletteColor(7);
	const Bit8PaletteColor Theme::fg4 = Bit8PaletteColor(8);

	const Bit8PaletteColor Theme::red = Bit8PaletteColor(9);
	const Bit8PaletteColor Theme::purple = Bit8PaletteColor(10);
	const Bit8PaletteColor Theme::green = Bit8PaletteColor(11);
	const Bit8PaletteColor Theme::greenSpeed = Bit8PaletteColor(12);
	const Bit8PaletteColor Theme::yellow = Bit8PaletteColor(13);
	const Bit8PaletteColor Theme::blue = Bit8PaletteColor(14);
	const Bit8PaletteColor Theme::ground = Bit8PaletteColor(15);
	const Bit8PaletteColor Theme::ground2 = Bit8PaletteColor(16);
	const Bit8PaletteColor Theme::sky = Bit8PaletteColor(17);
	const Bit8PaletteColor Theme::sky2 = Bit8PaletteColor(18);

	const PIXY10Font Theme::font = PIXY10Font();

	void Theme::apply(Button &button) {
		button.setHeight(elementHeight);
		button.setCornerRadius(cornerRadius);

		button.setBackgroundColor(&Theme::yellow);
		button.setPressedBackground(&Theme::purple);

		button.setForegroundColor(&Theme::fg1);
		button.setPressedForeground(&Theme::fg1);
	}

	void Theme::apply(Slider &slider) {
		slider.setHeight(elementHeight);
		slider.setCornerRadius(cornerRadius);

		slider.setBackgroundColor(&Theme::bg3);
		slider.setForegroundColor(&Theme::yellow);
	}
}
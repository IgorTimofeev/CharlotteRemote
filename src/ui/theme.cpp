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
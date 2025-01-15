//#include <Arduino.h>
//#include "ui/rc_application.h"
//
//using namespace pizdanc;
//
//RCApplication& application = RCApplication::getInstance();
//
//void setup() {
//	Serial.begin(115200);
//
//	application.setup();
//}
//
//void loop() {
//	application.tick();
//}

#include <Arduino.h>

#include "../lib/YOBA/src/yoba.h"
#include "../lib/YOBA/src/ui.h"
#include "../lib/YOBA/src/hardware/displays.h"
#include "../lib/YOBA/src/hardware/touchPanels.h"

#include "settings.h"
#include "ui/theme.h"

using namespace yoba;
using namespace yoba::hardware;
using namespace yoba::ui;
using namespace pizdanc;

ILI9341Display _display = ILI9341Display(
	ColorModel::Rgb565,
	RenderTargetOrientation::Clockwise270,
	settings::pinout::screen::chipSelect,
	settings::pinout::screen::dataCommand,
	settings::pinout::screen::reset
);

Bit8PaletteBufferedRenderer _renderer = Bit8PaletteBufferedRenderer(&_display, 32);

FT6336UTouchPanel _touchPanel = FT6336UTouchPanel(
	settings::pinout::screen::touch::interrupt,
	settings::pinout::screen::touch::reset
);

void setup() {
	Serial.begin(115200);

	_renderer.setup();
}

void loop() {
	Serial.println("pizda");

	_renderer.clear(&Theme::ground);
	_renderer.flush();
}
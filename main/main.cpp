//#include "rc.h"
//
//extern "C" void app_main(void) {
//	pizda::RC::getInstance().run();
//}

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include <YOBA/hardware/displays/ILI9341Display.h>
#include <YOBA/hardware/touchPanels/FT6336UTouchPanel.h>
#include <YOBA/resources/fonts/unscii16.h>
#include <esp_timer.h>
#include <esp_log.h>

#include "constants.h"
#include "resources/images/menuIconMFDNDImage.h"
#include "resources/images/menuIconMFDImage.h"

using namespace YOBA;
using namespace pizda;

ILI9341Display display = ILI9341Display(
	constants::spi::mosi,
	constants::spi::miso,
	constants::spi::sck,

	constants::screen::slaveSelect,
	constants::screen::dataCommand,
	constants::screen::reset,
	constants::screen::frequency,

	Size(240, 320)
);

RGB565PixelBufferRenderer renderer {};

Rgb565Color color0 = Rgb888Color(0x444444).toRgb565();
Rgb565Color color1 = Rgb888Color(0x00FF00).toRgb565();
Rgb565Color color2 = Rgb888Color(0x0000FF).toRgb565();
Rgb565Color color3 = Rgb888Color(0x000000).toRgb565();

unscii16Font font = unscii16Font();

MenuIconMFDImage image1 {};
MenuIconMFDNDImage image2 {};

uint32_t govnoTime = 0;

extern "C" void app_main(void) {
	display.setup();
	renderer.setTarget(&display);

	while (true) {
		// 1 - deltaTime
		// x - 1000000000

		auto fps = govnoTime > 0 ? 1'000'000 / govnoTime : 0;

		govnoTime = esp_timer_get_time();

		renderer.clear(&color0);
//		_renderer.renderFilledRectangle(Bounds(_display.getSize()), &color1);
		renderer.renderFilledRectangle(Bounds(10, 10, 30, 10), &color1);
		renderer.renderHorizontalLine(Point(10, 20), 130, &color2);
		renderer.renderVerticalLine(Point(10, 25), 130, &color3);

		renderer.renderString(Point(0, 0), &font, &color3, std::to_wstring(fps));
		renderer.renderString(Point(0, 300), &font, &color3, std::to_wstring(esp_get_free_heap_size() / 1024));

		renderer.renderImage(Point(5, 30), &image1);
		renderer.renderImage(Point(50, 30), &image2);

		renderer.flush();

		govnoTime = esp_timer_get_time() - govnoTime;

		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}
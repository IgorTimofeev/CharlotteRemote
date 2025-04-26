#include "rc.h"

extern "C" void app_main(void) {
	pizda::RC::getInstance().run();
}

//#include <YOBA/main.h>
//#include <YOBA/UI.h>
//#include <YOBA/hardware/displays/ILI9341Display.h>
//#include <YOBA/hardware/touchPanels/FT6336UTouchPanel.h>
//#include <YOBA/resources/fonts/unscii16.h>
//#include <esp_timer.h>
//#include <esp_log.h>
//
//#include "constants.h"
//
//using namespace YOBA;
//using namespace pizda;
//
//ILI9341Display _display = ILI9341Display(
//	constants::spi::mosi,
//	constants::spi::miso,
//	constants::spi::sck,
//
//	constants::screen::slaveSelect,
//	constants::screen::dataCommand,
//	constants::screen::reset,
//	constants::screen::frequency
//);
//
//Bit8PaletteRenderer _renderer = Bit8PaletteRenderer(4);
//
//PaletteColor color0 = PaletteColor(0);
//PaletteColor color1 = PaletteColor(1);
//PaletteColor color2 = PaletteColor(2);
//PaletteColor color3 = PaletteColor(3);
//
//unscii16Font font = unscii16Font();
//
//uint32_t govnoTime = 0;
//
//extern "C" void app_main(void) {
//	_display.setup();
//	_renderer.setTarget(&_display);
//
//	_renderer.setPaletteColors({
//		0xFF0000,
//		0x00FF00,
//		0x0000FF,
//		0x000000
//	});
//
//	ESP_LOGI("test", "getTransactionBufferHeight: %d", _renderer.getTransactionBufferHeight());
//	ESP_LOGI("test", "getTransactionBufferLength: %d", _renderer.getTransactionBufferLength());
//	ESP_LOGI("test", "getPaletteBufferLength: %d", _renderer.getPaletteBufferLength());
//
//	while (true) {
//		// 1 - deltaTime
//		// x - 1000000000
//
//		auto fps = govnoTime > 0 ? 1'000'000 / govnoTime : 0;
//
//		govnoTime = esp_timer_get_time();
//
//		_renderer.clear(&color0);
////		_renderer.renderFilledRectangle(Bounds(_display.getSize()), &color1);
//		_renderer.renderFilledRectangle(Bounds(10, 10, 30, 10), &color2);
//		_renderer.renderString(Point(0, 0), &font, &color3, std::to_wstring(fps));
//		_renderer.flush();
//
//		govnoTime = esp_timer_get_time() - govnoTime;
//
//		vTaskDelay(pdMS_TO_TICKS(1000));
//	}
//}
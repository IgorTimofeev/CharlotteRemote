#include <Arduino.h>
#include "ui/rc_application.h"

using namespace ui;

RCApplication& application = RCApplication::getInstance();

void setup() {
	Serial.begin(115200);
	SPI.begin();

	pinMode(2, OUTPUT);
	digitalWrite(2, 0);

	application.begin();
}

void loop() {
	application.tick();

	delay(1);
}

//#include <Arduino.h>
//#include "TFT_eSPI.h"
//
//TFT_eSPI _tft = TFT_eSPI(TFT_WIDTH, TFT_HEIGHT);
//TFT_eSprite _buffer = TFT_eSprite(&_tft);
//
//uint16_t _palette[16];
//
//void setup() {
//	Serial.begin(115200);
//
//	_tft.init();
//	_tft.setRotation(1);
//
////	_palette[0]  = 0x0000;
////	_palette[1]  = 0x1111;
////	_palette[2]  = 0x2222;
////	_palette[3]  = 0x3333;
////	_palette[4]  = 0x4444;
////	_palette[5]  = 0x5555;
////	_palette[6]  = 0x6666;
////	_palette[7]  = 0x7777;
////	_palette[8]  = 0x8888;
////	_palette[9]  = 0x9999;
////	_palette[10] = 0xAAAA;
////	_palette[11] = 0xBBBB;
////	_palette[12] = 0xCCCC;
////	_palette[13] = 0xDDDD;
////	_palette[14] = 0xEEEE;
////	_palette[15] = 0xFFFF;
//
//	_palette[0]  = _buffer.color24to16(0x000000);
//	_palette[1]  = _buffer.color24to16(0x111111);
//	_palette[2]  = _buffer.color24to16(0x222222);
//	_palette[3]  = _buffer.color24to16(0x333333);
//	_palette[4]  = _buffer.color24to16(0x444444);
//	_palette[5]  = _buffer.color24to16(0x555555);
//	_palette[6]  = _buffer.color24to16(0x666666);
//	_palette[7]  = _buffer.color24to16(0x777777);
//	_palette[8]  = _buffer.color24to16(0x888888);
//	_palette[9]  = _buffer.color24to16(0x999999);
//	_palette[10] = _buffer.color24to16(0xAAAAAA);
//	_palette[11] = _buffer.color24to16(0xBBBBBB);
//	_palette[12] = _buffer.color24to16(0xCCCCCC);
//	_palette[13] = _buffer.color24to16(0xDDDDDD);
//	_palette[14] = _buffer.color24to16(0xEEEEEE);
//	_palette[15] = _buffer.color24to16(0xFFFFFF);
//
//	_buffer.setColorDepth(4);
//	_buffer.createSprite(TFT_HEIGHT, TFT_WIDTH);
//	_buffer.createPalette(_palette);
//
//	_buffer.setTextColor(TFT_WHITE);
//
//	pinMode(25, GPIO_MODE_OUTPUT);
//}
//
//uint8_t _paletteIndex = 0;
//uint8_t _brightness = 0;
//
//void loop() {
//	_buffer.fillSprite(_paletteIndex);
//	_buffer.setCursor(0, 0);
//	_buffer.setTextSize(2);
//
//	_buffer.println("Total heap: " + String(ESP.getHeapSize() / 1024));
//	_buffer.println("Free heap: " + String(ESP.getFreeHeap() / 1024));
//	_buffer.println("Total PSRAM: " + String(ESP.getPsramSize() / 1024));
//	_buffer.println("Free PSRAM: " + String(ESP.getFreePsram() / 1024));
//	_buffer.println("Brightness: " + String(_brightness));
//
//	_buffer.pushSprite(0, 0);
//
////	_paletteIndex++;
////
////	if (_paletteIndex > 15)
////		_paletteIndex = 0;
//
//	delay(10);
//
//	dacWrite(25, _brightness);
//	_brightness += 1;
//
////	_tft.fillScreen(TFT_RED);
////	delay(1000);
////	_tft.fillScreen(TFT_BLACK);
////	delay(1000);
//}
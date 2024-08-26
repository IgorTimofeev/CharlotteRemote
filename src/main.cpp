#include <Arduino.h>
#include "ui/rc_application.h"

using namespace pizdanc;

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
//
//void setup() {
//	Serial.begin(115200);
//
//}
//
//void loop() {
//	Serial.println("pizda");
//}
#include <Arduino.h>
#include "ui/rc_application.h"

using namespace pizdanc;

RCApplication& application = RCApplication::getInstance();

void setup() {
	Serial.begin(115200);

	application.setup();
}

void loop() {
	application.tick();
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
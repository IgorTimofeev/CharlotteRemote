#include <Arduino.h>
#include "ui/rc.h"

using namespace pizdanc;

RC& rc = RC::getInstance();

void setup() {
	Serial.begin(115200);

	rc.setup();
}

void loop() {
	rc.tick();
}
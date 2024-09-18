#pragma once

#include "Arduino.h"
#include "settings.h"

using namespace pizdanc;

class OnboardLED {
	public:
		void begin() {
			pinMode(settings::pinout::board::led, OUTPUT);
			updateLED();
		}

		void blink() {
			_deadline = millis() + 5;
			updateLED();
		}

		void tick() {
			if (_deadline == 0 || millis() < _deadline)
				return;

			_deadline = 0;
			updateLED();
		}

	private:
		uint32_t _deadline = 0;

		void updateLED() const {
			digitalWrite(settings::pinout::board::led, _deadline > 0 ? HIGH : LOW);
		}
};
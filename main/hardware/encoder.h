#pragma once

#include "driver/gpio.h"
#include <esp_attr.h>

namespace pizda {
	class Encoder {
		public:
			Encoder(gpio_num_t clkPin, gpio_num_t dtPin, gpio_num_t swPin);

			void setup();

			bool wasInterrupted() const;
			void acknowledgeInterrupt();

			bool isPressed() const;

			int32_t getRotation() const;
			void setRotation(int32_t value);

		private:
			gpio_num_t
				_clkPin,
				_dtPin,
				_swPin;

			int32_t _rotation = 0;
			bool _interrupted = false;
			uint8_t _oldClkAndDt = 0;

			static void IRAM_ATTR clkDtInterruptHandler(void* args);
			static void IRAM_ATTR swInterruptHandler(void* args);
			void readRotation();
	};
}
#pragma once

#include "driver/gpio.h"
#include "main/event.h"
#include <esp_attr.h>
#include <functional>

namespace pizda {
	using namespace yoba;

	class EncoderRotateEvent : public Event {
		public:
			EncoderRotateEvent(int32_t rps) : Event(typeID), _RPS(rps) {

			}

			static uint16_t typeID;

			int32_t getRPS() const;

			int16_t getMappedRPS(uint16_t check1, int16_t value1, int16_t valueElse);
			int16_t getMappedRPS(uint16_t check1, uint16_t check2, int16_t value1, int16_t value2, int16_t valueElse);

		private:
			int32_t _RPS;
	};

	class EncoderPushEvent : public Event {
		public:
			EncoderPushEvent(bool down) : Event(typeID), _down(down) {

			}

			static uint16_t typeID;

			bool isDown() const;

		private:
			bool _down;
	};

	class Encoder {
		public:
			Encoder(gpio_num_t clkPin, gpio_num_t dtPin, gpio_num_t swPin);

			void setup();

			bool interrupted() const;
			void acknowledgeInterrupt();

			void setRotation(int32_t rotation);
			int32_t getRotation() const;

			bool isPressed() const;

		private:
			gpio_num_t
				_clkPin,
				_dtPin,
				_swPin;

			bool _interrupted = false;

			int32_t _rotation = 0;
			uint8_t _oldClkAndDt = 0;

			bool _pressed = false;

			static void IRAM_ATTR clkDtInterruptHandler(void* args);
			static void IRAM_ATTR swInterruptHandler(void* args);
			void readRotation();
			void readPressed();
	};
}
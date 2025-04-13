#pragma once

#include "driver/gpio.h"
#include "main/event.h"
#include <esp_attr.h>
#include <functional>

namespace pizda {
	using namespace yoba;

	class EncoderRotateEvent : public Event {
		public:
			EncoderRotateEvent(int32_t rps);

			static uint16_t typeID;

			int32_t getRPS() const;

			template<typename TValue>
			int16_t getRPSFactor(uint16_t check1, TValue value1, TValue valueElse);

			template<typename TValue>
			TValue getRPSFactor(uint16_t check1, TValue check2, TValue value1, TValue value2, TValue valueElse);

		private:
			int32_t _RPS;
	};

	template<typename TValue>
	int16_t EncoderRotateEvent::getRPSFactor(uint16_t check1, TValue value1, TValue valueElse) {
		TValue value;

		if (std::abs(_RPS) < check1) {
			value = value1;
		}
		else {
			value = valueElse;
		}

		return _RPS >= 0 ? value : -value;
	}

	template<typename TValue>
	TValue EncoderRotateEvent::getRPSFactor(uint16_t check1, TValue check2, TValue value1, TValue value2, TValue valueElse) {
		const auto absRPS = std::abs(_RPS);
		TValue value;

		if (absRPS < check1) {
			value = value1;
		}
		else if (absRPS < check2) {
			value = value2;
		}
		else {
			value = valueElse;
		}

		return _RPS >= 0 ? value : -value;
	}

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
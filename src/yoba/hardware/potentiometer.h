#pragma once

#include "Arduino.h"

namespace yoba {
	class Potentiometer {
		public:
			Potentiometer(uint8_t pin, uint16_t minValue, uint16_t maxValue, uint16_t minDelta) :
				_pin(pin),
				_minValue(minValue),
				_maxValue(maxValue),
				_minDelta(minDelta)
			{

			}

			void begin() {
				pinMode(_pin, INPUT_PULLUP);

				_oldValue = readClamped();
			}

			uint16_t read() const {
				return analogRead(_pin);
			}

			uint16_t readClamped() const {
				return max(min(read(), _maxValue), _minValue);
			}

			uint16_t readSmooth() {
				auto value = readClamped();

				if (abs(value - _oldValue) < _minDelta)
					return _oldValue;

				const auto newValueFactor = 0.5f;

				_oldValue = (uint16_t) ((float) value * newValueFactor + (float) _oldValue * (1 - newValueFactor));

				return _oldValue;
			}

			float readSmoothFloat() {
				return (float) (readSmooth() - _minValue) / (float) (_maxValue - _minValue);
			}

		private:
			uint16_t _oldValue = 0;
			uint16_t _minValue;
			uint16_t _maxValue;
			uint16_t _minDelta;
			uint8_t _pin;
	};
}

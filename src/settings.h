#pragma once

#include "cstdint"

namespace pizdanc {
	namespace settings {
		namespace pinout {
			namespace board {
				static const uint8_t led = 2;
			}

			namespace screen {
				namespace touch {
					static const uint8_t reset = 32;
					static const uint8_t interrupt = 26;
					static const uint8_t sda = 21;
					static const uint8_t scl = 22;
				}

				static const uint8_t chipSelect = 5;
				static const uint8_t dataCommand = 16;
				static const uint8_t reset = 17;
				static const uint32_t frequency = 40000000;
			}

			namespace transceiver {
				static const uint8_t chipSelect = 4;
				static const uint8_t reset = 33;
				static const uint8_t busy = 25;
				static const uint8_t dio0 = 35;
			}
		}

		namespace application {
			static const uint32_t tickInterval = 1000 / 60;
		}

		namespace transceiver {
			static const uint32_t tickInterval = 1000 / 60;

			static const uint32_t packetHeader = 0x506C416E;
			static const float frequency = 915;
			static const float bandwidth = 500;
			static const uint8_t spreadingFactor = 7;
			static const uint8_t codingRate = 5;
			static const uint16_t power = 22;
			static const uint16_t preambleLength = 8;
		}

		namespace screen {

		}
	}
}

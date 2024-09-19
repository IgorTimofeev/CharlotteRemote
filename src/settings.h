#pragma once

#include "cstdint"

namespace pizdanc {
	namespace settings {
		namespace pinout {
			namespace board {
				const uint8_t led = 2;
			}

			namespace transceiver {
				const uint8_t chipSelect = 4;
				const uint8_t reset = 33;
				const uint8_t busy = 25;
				const uint8_t dio0 = 35;
			}

			namespace screen {
				namespace touch {
					const uint8_t reset = 32;
					const uint8_t interrupt = 26;
				}

				const uint8_t chipSelect = 5;
				const uint8_t dataCommand = 16;
				const uint8_t reset = 17;
			}
		}

		namespace application {
			const uint32_t tickInterval = 1000 / 60;
		}

		namespace transceiver {
			const uint32_t tickInterval = 1000 / 60;

			const uint32_t packetHeader = 0x506C416E;
			const float frequency = 915;
			const float bandwidth = 500;
			const uint8_t spreadingFactor = 7;
			const uint8_t codingRate = 5;
			const uint16_t power = 22;
			const uint16_t preambleLength = 8;
		}

		namespace screen {

		}
	}
}

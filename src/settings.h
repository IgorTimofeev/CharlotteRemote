#pragma once

#ifndef PULLTICK_SETTINGS_H
#define PULLTICK_SETTINGS_H
#endif

#include "cstdint"
#include "yoba/color.h"

using namespace yoba;

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
			PROGMEM const uint16_t palette[16] {
				// Background
				TrueColor(0x000000).to16Bit(),
				TrueColor(0x111111).to16Bit(),
				TrueColor(0x222222).to16Bit(),
				TrueColor(0x333333).to16Bit(),
				TrueColor(0x444444).to16Bit(),

				// Foreground
				TrueColor(0xFFFFFF).to16Bit(),
				TrueColor(0xDDDDDD).to16Bit(),
				TrueColor(0xBBBBBB).to16Bit(),
				TrueColor(0x999999).to16Bit(),

				// Red
				TrueColor(0xff0000).to16Bit(),
				// Purple
				TrueColor(0xec04ee).to16Bit(),
				// Green
				TrueColor(0x008100).to16Bit(),
				// Yellow
				TrueColor(0xffd200).to16Bit(),
				// Ocean
				TrueColor(0x00ffff).to16Bit(),
				// Ground
				TrueColor(0x97b838).to16Bit(),
				// Sky
				TrueColor(0x317fcb).to16Bit(),
			};
		}
	}
}

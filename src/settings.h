#pragma once

#ifndef PULLTICK_SETTINGS_H
#define PULLTICK_SETTINGS_H
#endif

#include "cstdint"

namespace pizdanc {
	namespace settings {
		namespace pinout {
			namespace transceiver {
				constexpr uint8_t chipSelect = 4;
				constexpr uint8_t reset = 33;
				constexpr uint8_t busy = 25;
			}

			namespace screen {
				namespace touch {
					constexpr uint8_t reset = 32;
					constexpr uint8_t interrupt = 26;
				}
			}
		}

		namespace transceiver {
			constexpr uint32_t packetHeader = 0x506C416E;
		}
	}
}

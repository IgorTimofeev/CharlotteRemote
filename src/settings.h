#pragma once

#ifndef PULLTICK_SETTINGS_H
#define PULLTICK_SETTINGS_H
#endif

#include "cstdint"

namespace pizdanc {
	class Settings {
		public:
			class Pinout {
				public:
					class Transceiver {
						public:
							static const uint8_t chipSelect = 32;
							static const uint8_t reset = 25;
							static const uint8_t busy = 33;
					};
			};

			class Transceiver {
				public:
					static const uint32_t packetHeader = 0x506C416E;
			};
	};
}


#pragma once

#include <aes/esp_aes.h>
#include "Arduino.h"
#include "Ra01S.h"
#include "packet.h"
#include "settings.h"

namespace pizdanc {
	class RCApplication;

	class Transceiver {
		public:
			Transceiver() = default;

			void begin();
			void tick(RCApplication& application);

		private:
			SX126x _sx1262 = SX126x(
				settings::pinout::transceiver::chipSelect,
				settings::pinout::transceiver::reset,
				settings::pinout::transceiver::busy
			);

			uint8_t _sx1262Buffer[255] {};

			esp_aes_context _AESContext = esp_aes_context();
			uint8_t _AESKey[16] = { 0x02, 0xEB, 0x46, 0x45, 0x96, 0xB0, 0xD6, 0xB9, 0x7C, 0x34, 0xBE, 0x77, 0x75, 0xF2, 0xBE, 0x1B };
			uint8_t _AESIV[16] = { 0x17, 0x5D, 0x25, 0x2A, 0xFD, 0x72, 0x1E, 0x01, 0x02, 0x60, 0x88, 0x92, 0x9A, 0x9B, 0x2A, 0xA9 };
			uint8_t _AESIVCopy[sizeof(_AESIV)] {};
			uint8_t _AESBuffer[sizeof(_sx1262Buffer)] {};

			// Received Signal Strength Indicator
			int8_t _rssi = 0;
			// Signal-to-Noise Ratio
			int8_t _snr = 0;

			uint32_t _deadline = 0;

			template<typename T>
			void send(PacketType packetType, const T& packet);
			void receive(RCApplication &application);
			void parsePacket(RCApplication &application, uint8_t* bufferPtr);
	};
}
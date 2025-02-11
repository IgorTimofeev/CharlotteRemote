
#pragma once

#include "RadioLib.h"
#include "radioLibESPHAL.h"
#include "packet.h"
#include "../../constants.h"

namespace pizda {
	enum TransceiverMode : uint8_t {
		Transmit,
		StartReceive,
		Receive
	};

	class Transceiver {
		public:
			void setup();
			void tick();

			float getRssi() const;
			float getSnr() const;

		private:
			EspHal _hal = EspHal(5, 19, 27);

			SX1262 _sx1262 = new Module(
				&_hal,
				constants::hardware::transceiver::chipSelect,
				constants::hardware::transceiver::dio0,
				constants::hardware::transceiver::reset,
				constants::hardware::transceiver::busy
			);

			TransceiverMode _mode = TransceiverMode::StartReceive;
			static volatile bool _canOperate;
			IRAM_ATTR static void onDio1Action();
			uint32_t _tickDeadline = 0;

			uint8_t _sx1262Buffer[256] {};

			uint8_t _aesKey[16] = {0x02, 0xEB, 0x46, 0x45, 0x96, 0xB0, 0xD6, 0xB9, 0x7C, 0x34, 0xBE, 0x77, 0x75, 0xF2, 0xBE, 0x1B };
			uint8_t _aesIV[16] = {0x17, 0x5D, 0x25, 0x2A, 0xFD, 0x72, 0x1E, 0x01, 0x02, 0x60, 0x88, 0x92, 0x9A, 0x9B, 0x2A, 0xA9 };
			uint8_t _aesIVCopy[sizeof(_aesIV)] {};
			uint8_t _aesBuffer[sizeof(_sx1262Buffer)] {};

			// Received Signal Strength Indicator
			float _rssi = 0;
			// Signal-to-Noise Ratio
			float _snr = 0;

			uint32_t _deadline = 0;

			template<typename T>
			void send(PacketType packetType, const T& packet);
			void startReceive();
			void receive();
			void parsePacket(uint8_t* bufferPtr);
	};
}
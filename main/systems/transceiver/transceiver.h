#pragma once

#include <cstdint>

namespace pizda {
	class Transceiver {
		public:
			virtual ~Transceiver() = default;

			virtual bool finishSpectrumScanning() = 0;
			virtual bool transmit(const uint8_t* buffer, uint8_t length, uint32_t timeoutUs) = 0;
			virtual bool receive(uint8_t* buffer, uint8_t& length, uint32_t timeoutUs) = 0;

			virtual bool beginSpectrumScanning() = 0;
			virtual bool getSpectrumScanningRecord(const uint32_t frequencyMHz, int8_t& RSSI) = 0;
	};
}
#pragma once

#include "cstdint"

namespace pizda {
	#pragma pack(push, 1)
	
	enum class PacketType : uint8_t {
		remoteChannelDataStructure,
		remoteChannelData,
		remoteMotorConfiguration,
		remoteBaro,
		
		aircraftADIRS
	};
	
	class Packet {
		public:
			constexpr static uint8_t typeLengthBits = 4;
			constexpr static uint8_t checksumLengthBytes = 1;
		
		private:
		
	};

	#pragma pack(pop)
}
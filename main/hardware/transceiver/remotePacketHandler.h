#pragma once

#include "packetHandler.h"

#include <cmath>
#include <functional>

#include <bitStream.h>

namespace pizda {
	class RemotePacketHandler : public PacketHandler {
		public:
			void onConnectionLost() override;
			void onConnectionRestored() override;

		protected:
			bool readPacket(BitStream& stream, PacketType packetType, uint8_t payloadLength) override;
			bool writePacket(BitStream& stream, PacketType packetType) override;
		
		private:
			uint32_t _aircraftADIRSPacketTime = 0;
			
			bool readAircraftADIRSPacket(BitStream& stream, uint8_t payloadLength);
	};
}
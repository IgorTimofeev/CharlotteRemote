#pragma once

#include "packetHandler.h"

#include <cmath>
#include <functional>

#include <esp_log.h>

#include <bitStream.h>

namespace pizda {
	class RemotePacketHandler : public PacketHandler {
		public:
			void onConnectionStateChanged(TransceiverConnectionState fromState, TransceiverConnectionState toState) override;

		protected:
			bool readPacket(BitStream& stream, PacketType packetType, uint8_t payloadLength) override;
			bool writePacket(BitStream& stream, PacketType packetType) override;
		
		private:
			uint32_t _aircraftADIRSPacketTime = 0;
			
			bool readAircraftADIRSPacket(BitStream& stream, uint8_t payloadLength);
			bool writeRemoteBaroPacket(BitStream& stream);
			
			template<typename T>
			float sanitizeValue(T value, T min, T max) {
				if (value < min) {
					ESP_LOGW(_logTag, "value %f is out of range [%f, %f]", static_cast<float>(value), static_cast<float>(min), static_cast<float>(max));
					
					value = min;
					
				}
				else if (value > max) {
					ESP_LOGW(_logTag, "value %f is out of range [%f, %f]", static_cast<float>(value), static_cast<float>(min), static_cast<float>(max));
					
					value = max;
				}
				
				return value;
			}
	};
}
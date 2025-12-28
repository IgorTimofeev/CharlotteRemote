#pragma once

#include "packetHandler.h"

#include <cmath>
#include <functional>

#include <esp_log.h>

#include <bitStream.h>

namespace pizda {
	class RemotePacketHandler : public PacketHandler {
		public:
			RemotePacketHandler() : PacketHandler({
				PacketType::remoteChannelsData,
				PacketType::remoteChannelsData,
				PacketType::remoteChannelsData,
				
				PacketType::remoteAuxiliary,
				
				PacketType::remoteChannelsData,
				PacketType::remoteChannelsData,
				PacketType::remoteChannelsData,
				
				std::nullopt
			}) {
			
			}
			
			void onConnectionStateChanged(TransceiverConnectionState fromState, TransceiverConnectionState toState) override;

		protected:
			bool readPacket(BitStream& stream, PacketType packetType, uint8_t payloadLength) override;
			bool writePacket(BitStream& stream, PacketType packetType) override;
			
		private:
			uint8_t _packetIndex = 0;
			int64_t _aircraftADIRSPacketTime = 0;
			
			bool readAircraftADIRSPacket(BitStream& stream, uint8_t payloadLength);
			bool readAircraftStatisticsPacket(BitStream& stream, uint8_t payloadLength);
			bool readAircraftAutopilotPacket(BitStream& stream, uint8_t payloadLength);
			
			bool writeRemoteChannelsDataPacket(BitStream& stream);
			bool writeRemoteAuxiliary0Packet(BitStream& stream);
			
			template<typename T>
			static float sanitizeValue(T value, T min, T max) {
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
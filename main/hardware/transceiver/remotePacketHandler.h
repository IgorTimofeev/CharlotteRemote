#pragma once

#include "packetHandler.h"

#include <cmath>
#include <functional>

#include <esp_log.h>

#include <bitStream.h>

namespace pizda {
	class PacketSequenceItem {
		public:
			PacketSequenceItem(RemotePacketType type, uint8_t count, bool useEnqueued = false);
			RemotePacketType getType() const;
			uint8_t getCount() const;
			bool useEnqueued() const;
		
		private:
			RemotePacketType _type;
			uint8_t _count;
			bool _useEnqueued;
	};
	
	class RemotePacketHandler : public PacketHandler<RemoteState, RemotePacketType, AircraftState, AircraftPacketType> {
		public:
			void enqueue(RemotePacketType type) {
				_packetQueue.push(type);
			}
			
		protected:
			[[noreturn]] void onStart() override;
			RemotePacketType getTransmitPacketType() override;
			bool onTransmit(BitStream& stream, RemotePacketType packetType) override;
			bool onReceive(BitStream& stream, AircraftPacketType packetType, uint8_t payloadLength) override;
			void onIsConnectedChanged() override;
			
		private:
			int64_t _aircraftADIRSPacketTime = 0;
			
			std::vector<PacketSequenceItem> _packetSequence {
				PacketSequenceItem(RemotePacketType::channelsData, 3),
				PacketSequenceItem(RemotePacketType::auxiliary, 1, true)
			};
			
			uint8_t _packetSequenceIndex = 0;
			uint8_t _packetSequenceItemCounter = 0;
			
			std::queue<RemotePacketType> _packetQueue {};

			bool receiveAircraftCalibrationPacket(BitStream& stream, uint8_t payloadLength);
			bool receiveAircraftADIRSPacket(BitStream& stream, uint8_t payloadLength);
			bool receiveAircraftAuxiliaryPacket(BitStream& stream, uint8_t payloadLength);
			
			bool transmitNOPPacket(BitStream& stream);
			bool transmitRemoteChannelsDataPacket(BitStream& stream);
			bool transmitRemoteAuxiliaryPacket(BitStream& stream);
	
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
			
			static float readRadians(BitStream& stream, float range, uint8_t bits);;
			static void writeRadians(BitStream& stream, float value, float range, uint8_t bits);
	};
}
#pragma once

#include "systems/communicationManager/communicationManager.h"

#include <queue>
#include <set>

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
	
	class RemoteCommunicationManager : public CommunicationManager<RemotePacketType, AircraftPacketType> {
		public:
			void enqueue(RemotePacketType type);
			
		protected:
			[[noreturn]] void onStart() override;
			RemotePacketType getTransmitPacketType() override;
			bool onTransmit(BitStream& stream, RemotePacketType packetType) override;
			bool onReceive(BitStream& stream, AircraftPacketType packetType, uint8_t payloadLength) override;
			void onConnectionStateChanged() override;
			
		private:
			constexpr static uint32_t _trendsInterval = 500'000;
			int64_t _trendsTime = 0;
			float _trendsAirspeedPrevMPS = 0;
			float _trendsAltitudePrevM = 0;
			
			std::vector<PacketSequenceItem> _packetSequence {
				PacketSequenceItem(RemotePacketType::controls, 2),
				PacketSequenceItem(RemotePacketType::controls, 1, true)
			};
			
			uint8_t _packetSequenceIndex = 0;
			uint8_t _packetSequenceItemCounter = 0;
			
			std::set<RemotePacketType> _enqueuedPackets {};

			bool receiveAircraftADIRSPacket(BitStream& stream, uint8_t payloadLength);
			bool receiveAircraftAuxiliaryPacket(BitStream& stream, uint8_t payloadLength);
			bool receiveAircraftCalibrationPacket(BitStream& stream, uint8_t payloadLength);
			
			void transmitRemoteControlsPacket(BitStream& stream);
			void transmitRemoteTrimPacket(BitStream& settingsValue);
			void transmitRemoteLightsPacket(BitStream& stream);
			void transmitRemoteBaroPacket(BitStream& stream);
			void transmitRemoteAutopilotPacket(BitStream& stream);
			void transmitRemoteMotorConfigurationPacket(BitStream& stream);
			void transmitRemoteCalibratePacket(BitStream& stream);

			void onSpectrumScanning();
	};
}
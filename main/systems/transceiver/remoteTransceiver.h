#pragma once

#include <array>

#include <bitStream.h>

#include "systems/transceiver/transceiver.h"

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
	
	class RemoteTransceiver : public Transceiver<
		RemotePacketType,
		RemotePacket::typeLengthBits,

		AircraftPacketType,
		AircraftPacket::typeLengthBits
	> {
		public:
			void enqueueAuxiliary(RemoteAuxiliaryPacketType type);

		protected:
			[[noreturn]] void onStart() override;
			RemotePacketType getTransmitPacketType() override;
			void onTransmit(BitStream& stream, RemotePacketType packetType) override;
			bool onReceive(BitStream& stream, AircraftPacketType packetType, uint8_t payloadLength) override;
			void onConnectionStateChanged() override;

		private:
			constexpr static uint32_t _trendsInterval = 500'000;
			int64_t _trendsTime = 0;
			float _trendsAirspeedPrevMPS = 0;
			float _trendsAltitudePrevM = 0;

			std::array<PacketSequenceItem, 2> _packetSequence {
				PacketSequenceItem(RemotePacketType::controls, 2),
				PacketSequenceItem(RemotePacketType::controls, 1, true)
			};

			uint8_t _packetSequenceIndex = 0;
			uint8_t _packetSequenceItemCounter = 0;

			// FIFO packet queue
			int16_t _packetQueueIndex = -1;
			RemoteAuxiliaryPacketType _packetQueue[255] {};
			RemoteAuxiliaryPacketType _auxiliaryPacketType = RemoteAuxiliaryPacketType::lights;

			bool receiveAircraftTelemetryPrimaryPacket(BitStream& stream, uint8_t payloadLength);
			bool receiveAircraftTelemetrySecondaryPacket(BitStream& stream, uint8_t payloadLength);
			bool receiveAircraftAuxiliaryPacket(BitStream& stream, uint8_t payloadLength);
			bool receiveAircraftAuxiliaryCalibrationPacket(BitStream& stream, uint8_t payloadLength);

			void transmitRemoteControlsPacket(BitStream& stream);
			void transmitRemoteAuxiliaryPacket(BitStream& stream);
			void transmitRemoteAuxiliaryTrimPacket(BitStream& stream);
			void transmitRemoteAuxiliaryLightsPacket(BitStream& stream);
			void transmitRemoteAuxiliaryBaroPacket(BitStream& stream);
			void transmitRemoteAuxiliaryAutopilotPacket(BitStream& stream);
			void transmitRemoteAuxiliaryMotorConfigurationPacket(BitStream& stream);
			void transmitRemoteAuxiliaryCalibratePacket(BitStream& stream);

			uint16_t _spectrumScanningHistoryIndex = 0;
			int64_t _spectrumScanningSampleRSSISum = 0;
			uint32_t _spectrumScanningSampleQuantity = 0;

			bool stopSpectrumScanning();

			void onSpectrumScanning();
	};
}
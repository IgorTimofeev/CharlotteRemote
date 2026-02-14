#pragma once

#include <bitStream.h>

#include "systems/transceiver/transceiver.h"

namespace pizda {
	using namespace YOBA;

	class RemoteTransceiver : public Transceiver<
		RemotePacketType,
		RemotePacket::typeLengthBits,

		2,
		RemoteAuxiliaryPacketType,

		AircraftPacketType,
		AircraftPacket::typeLengthBits,

		1
	> {
		public:
			RemoteTransceiver();

			bool setup() override;

			void getPPS(uint16_t& RXPPS, uint16_t& TXPPS) const;

		protected:
			[[noreturn]] void onStart() override;
			void onTransmit(BitStream& stream, RemotePacketType packetType) override;
			bool onReceive(BitStream& stream, AircraftPacketType packetType, uint8_t payloadLength) override;
			void onConnectionStateChanged() override;

		private:
			constexpr static uint32_t trendsInterval = 500'000;
			int64_t _trendsTime = 0;
			float _trendsAirspeedPrevMPS = 0;
			float _trendsAltitudePrevM = 0;
			int64_t _PPSTime = 0;
			uint16_t _RXPPSTemp = 0;
			uint16_t _TXPPSTemp = 0;
			uint16_t _RXPPS = 0;
			uint16_t _TXPPS = 0;

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
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

			int8_t getRSSI() const;
			int8_t getSNR() const;

		protected:
			[[noreturn]] void onStart() override;
			void onTransmit(BitStream& stream, RemotePacketType packetType) override;
			bool onReceive(BitStream& stream, AircraftPacketType packetType, uint8_t payloadLength) override;
			void onConnectionStateChanged() override;

		private:
			constexpr static uint32_t trendsInterval = 500'000;
			int64_t _trendsTime = 0;
			float _trendsYawPrevRad = 0;
			float _trendsAirspeedPrevMPS = 0;
			float _trendsAltitudePrevM = 0;

			constexpr static uint8_t _RSSIFrequencyHz = 1;
			constexpr static uint32_t _RSSIAndSNRUpdateIntervalUs = 1'000'000 / _RSSIFrequencyHz;
			int64_t _RSSIAndSNRUpdateTimeUs = 0;
			int8_t _RSSI = 0;
			int8_t _SNR = 0;

			int64_t _communicationSettingsACKTime = 0;

			bool receiveAircraftTelemetryPrimaryPacket(BitStream& stream, uint8_t payloadLength);
			bool receiveAircraftTelemetrySecondaryPacket(BitStream& stream, uint8_t payloadLength);
			bool receiveAircraftAuxiliaryPacket(BitStream& stream, uint8_t payloadLength);
			bool receiveAircraftAuxiliaryCalibrationPacket(BitStream& stream, uint8_t payloadLength);
			bool receiveAircraftAuxiliaryXCVRACKPacket(BitStream& stream, uint8_t payloadLength);

			void transmitRemoteControlsPacket(BitStream& stream);
			void transmitRemoteAuxiliaryPacket(BitStream& stream);
			void transmitRemoteAuxiliaryTrimPacket(BitStream& stream);
			void transmitRemoteAuxiliaryLightsPacket(BitStream& stream);
			void transmitRemoteAuxiliaryBaroPacket(BitStream& stream);
			void transmitRemoteAuxiliaryAutopilotPacket(BitStream& stream);
			void transmitRemoteAuxiliaryMotorsPacket(BitStream& stream);
			void transmitRemoteAuxiliaryCalibratePacket(BitStream& stream);
			void transmitRemoteAuxiliaryADIRSPacket(BitStream& stream);
			void transmitRemoteAuxiliaryXCVRPacket(BitStream& stream);
			void transmitRemoteAuxiliaryPIDPacket(BitStream& stream);

			uint16_t _spectrumScanningHistoryIndex = 0;
			int64_t _spectrumScanningSampleRSSISum = 0;
			uint32_t _spectrumScanningSampleCount = 0;

			bool stopSpectrumScanning();
			void onSpectrumScanning();
	};
}
#pragma once

#include <bitStream.h>

#include "systems/transceiver/transceiver.h"

namespace pizda {
	using namespace YOBA;

	class RemoteTransceiver : public Transceiver<
		RemotePacketType,
		RemotePacket::typeLengthBits,

		3,
		RemoteSystemPacketType,

		AircraftPacketType,
		AircraftPacket::typeLengthBits,

		0
	> {
		public:
			RemoteTransceiver();

			bool setup() override;

			int8_t getRSSI() const;
			int8_t getSNR() const;

		protected:
			void onTick() override;
			void onTransmit(BitStream& stream, RemotePacketType packetType) override;
			bool onReceive(BitStream& stream, AircraftPacketType packetType, uint8_t payloadLength) override;
			void onConnectionStateChanged() override;

		private:
			// ----------------------------- Trends -----------------------------

			constexpr static uint32_t trendsInterval = 500'000;
			int64_t _trendsTime = 0;
			float _trendsYawPrevRad = 0;
			float _trendsAirspeedPrevMPS = 0;
			float _trendsAltitudePrevM = 0;

			// ----------------------------- RSSI / SNR -----------------------------

			constexpr static uint8_t _RSSIFrequencyHz = 1;
			constexpr static uint32_t _RSSIAndSNRUpdateIntervalUs = 1'000'000 / _RSSIFrequencyHz;
			int64_t _RSSIAndSNRUpdateTimeUs = 0;
			int8_t _RSSI = 0;
			int8_t _SNR = 0;

			void updateRSSIAndSNR();

			// ----------------------------- Communication settings -----------------------------

			void onCommunicationSettingsSyncCheckScheduled() override;

			void onCommunicationSettingsSyncCheckCompleted() override;

			// ----------------------------- Spectrum scanning -----------------------------

			uint16_t _spectrumScanningHistoryIndex = 0;
			int64_t _spectrumScanningSampleRSSISum = 0;
			uint32_t _spectrumScanningSampleCount = 0;

			bool stopSpectrumScanning();
			void spectrumScanningTick();

			// ----------------------------- Other stuff -----------------------------

			static void writePID(BitStream& stream, const PIDCoefficients& coefficients);
			void transmitRemoteSystemPacket(BitStream& stream) const;
	};
}
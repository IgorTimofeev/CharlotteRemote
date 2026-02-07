#pragma once

#include <NVSSettings.h>
#include <SX1262.h>

#include "config.h"

namespace pizda {
	using namespace YOBA;

	class TransceiverSettingsCommunication {
		public:
			uint32_t RFFrequencyHz = config::transceiver::RFFrequencyHz;
			SX1262::LoRaBandwidth bandwidth = config::transceiver::bandwidth;
			uint8_t spreadingFactor = config::transceiver::spreadingFactor;
			SX1262::LoRaCodingRate codingRate = config::transceiver::codingRate;
			uint8_t syncWord = config::transceiver::syncWord;
			int8_t powerDBm = config::transceiver::powerDBm;
			uint16_t preambleLength = config::transceiver::preambleLength;

			void sanitize() {
				RFFrequencyHz = std::clamp<uint32_t>(RFFrequencyHz, 120'000'000, 960'000'000);
				spreadingFactor = std::clamp<uint8_t>(spreadingFactor, 5, 12);
				powerDBm = std::clamp<int8_t>(powerDBm, -16, 22);
			}
	};

	class TransceiverSettingsSpectrumScanningFrequency {
		public:
			uint32_t from;
			uint32_t to;
			uint32_t step;
	};

	class TransceiverSettingsSpectrumScanning {
		public:
			TransceiverSettingsSpectrumScanningFrequency frequency {};
	};

	class TransceiverSettings : public NVSSettings {
		public:
			TransceiverSettingsCommunication communication {};
			TransceiverSettingsSpectrumScanning spectrumScanning {};

		protected:
			const char* getNamespace() override {
				return _namespace;
			}

			void onRead(const NVSStream& stream) override {
				spectrumScanning.frequency.from = stream.readUint32(_spectrumScanningFrequencyFrom, 430'000'000);
				spectrumScanning.frequency.to = stream.readUint32(_spectrumScanningFrequencyTo, 440'000'000);
				spectrumScanning.frequency.step = stream.readUint32(_spectrumScanningFrequencyStep, 10'000);

				communication.RFFrequencyHz = stream.readUint32(_communicationRFFrequencyHz, config::transceiver::RFFrequencyHz);
				communication.bandwidth = static_cast<SX1262::LoRaBandwidth>(stream.readUint8(_communicationBandwidth, static_cast<uint8_t>(config::transceiver::bandwidth)));
				communication.spreadingFactor = stream.readUint8(_communicationSpreadingFactor, config::transceiver::spreadingFactor);
				communication.codingRate = static_cast<SX1262::LoRaCodingRate>(stream.readUint8(_communicationCodingRate,  static_cast<uint8_t>(config::transceiver::codingRate)));
				communication.syncWord = stream.readUint8(_communicationSyncWord, config::transceiver::syncWord);
				communication.powerDBm = stream.readInt8(_communicationPowerDBm, config::transceiver::powerDBm);
				communication.preambleLength = stream.readUint16(_communicationPreambleLength, config::transceiver::preambleLength);
			}

			void onWrite(const NVSStream& stream) override {
				stream.writeUint32(_spectrumScanningFrequencyFrom, spectrumScanning.frequency.from);
				stream.writeUint32(_spectrumScanningFrequencyTo, spectrumScanning.frequency.to);
				stream.writeUint32(_spectrumScanningFrequencyStep, spectrumScanning.frequency.step);

				stream.writeUint32(_communicationRFFrequencyHz, communication.RFFrequencyHz);
				stream.writeUint8(_communicationBandwidth, static_cast<uint8_t>(communication.bandwidth));
				stream.writeUint8(_communicationSpreadingFactor, communication.spreadingFactor);
				stream.writeUint8(_communicationCodingRate, static_cast<uint8_t>(communication.codingRate));
				stream.writeUint8(_communicationSyncWord, communication.syncWord);
				stream.writeInt8(_communicationPowerDBm, communication.powerDBm);
				stream.writeUint16(_communicationPreambleLength, communication.preambleLength);
			}

		private:
			constexpr static auto _namespace = "trn";
			
			constexpr static auto _spectrumScanningFrequencyFrom  = "ssff";
			constexpr static auto _spectrumScanningFrequencyTo  = "ssft";
			constexpr static auto _spectrumScanningFrequencyStep  = "ssfs";

			constexpr static auto _communicationRFFrequencyHz = "cmrf";
			constexpr static auto _communicationBandwidth = "cmbw";
			constexpr static auto _communicationSpreadingFactor = "cmsf";
			constexpr static auto _communicationCodingRate = "cmcr";
			constexpr static auto _communicationSyncWord = "cmsw";
			constexpr static auto _communicationPowerDBm = "cmpw";
			constexpr static auto _communicationPreambleLength = "cmpl";
	};
}

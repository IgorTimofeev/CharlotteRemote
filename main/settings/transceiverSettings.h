#pragma once

#include <NVSSettings.h>

namespace pizda {
	using namespace YOBA;

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

	class TransceiverSettings: public NVSSettings {
		public:
			TransceiverSettingsSpectrumScanning spectrumScanning {};

		protected:
			const char* getNamespace() override {
				return _namespace;
			}

			void onRead(const NVSStream& stream) override {
				spectrumScanning.frequency.from = stream.readUint32(_spectrumScanningFrequencyFrom, 430'000'000);
				spectrumScanning.frequency.to = stream.readUint32(_spectrumScanningFrequencyTo, 440'000'000);
				spectrumScanning.frequency.step = stream.readUint32(_spectrumScanningFrequencyStep, 10'000);
			}

			void onWrite(const NVSStream& stream) override {
				stream.writeUint32(_spectrumScanningFrequencyFrom, spectrumScanning.frequency.from);
				stream.writeUint32(_spectrumScanningFrequencyTo, spectrumScanning.frequency.to);
				stream.writeUint32(_spectrumScanningFrequencyStep, spectrumScanning.frequency.step);
			}

		private:
			constexpr static auto _namespace = "trn";
			
			constexpr static auto _spectrumScanningFrequencyFrom  = "ssff";
			constexpr static auto _spectrumScanningFrequencyTo  = "ssft";
			constexpr static auto _spectrumScanningFrequencyStep  = "ssfs";
	};
}

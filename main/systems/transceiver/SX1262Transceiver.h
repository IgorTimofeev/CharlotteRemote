#pragma once

#include <cstdint>
#include <cmath>
#include <utility>

#include <esp_log.h>
#include <esp_timer.h>

#include <SX1262.h>

#include "systems/transceiver/transceiver.h"
#include "config.h"

namespace pizda {
	using namespace YOBA;

	class SX1262Transceiver : public Transceiver {
		public:
			bool setup() {
				const auto error = _SX.setup(
					config::spi::device,
					config::transceiver::SPIFrequencyHz,
					
					config::transceiver::SS,
					config::transceiver::RST,
					config::transceiver::busy,
					config::transceiver::DIO1,
					
					config::transceiver::RFFrequencyHz,
					config::transceiver::bandwidth,
					config::transceiver::spreadingFactor,
					config::transceiver::codingRate,
					config::transceiver::syncWord,
					config::transceiver::powerDBm,
					config::transceiver::preambleLength
				);
				
				if (error != SX1262Error::none) {
					ESP_LOGE(_logTag, "SX1262 setup failed with code %d", std::to_underlying(error));
					
					return false;
				}
				
				return true;
			}

			float getSNR() const {
				return _SNR;
			}
			
			float getRSSI() const {
				return _RSSI;
			}
			
			bool transmit(const uint8_t* buffer, const uint8_t length, const uint32_t timeoutUs) override {
//		ESP_LOGI(_logTag, "write length: %d", length);
//
//		for (int i = 0; i < length; ++i) {
//			ESP_LOGI(_logTag, "write buffer[%d]: %d", i, _buffer[i]);
//		}

				const auto error = _SX.transmit(buffer, length, timeoutUs);

				if (error == SX1262Error::none)
					return true;

				logError("transmit error", error);

				return false;
			}

			bool receive(uint8_t* buffer, uint8_t& length, const uint32_t timeoutUs) override {
				const auto error = _SX.receive(buffer, length, timeoutUs);

				if (error == SX1262Error::none) {
					//		ESP_LOGI(_logTag, "read length: %d", length);
					//
					//		for (int i = 0; i < length; ++i) {
					//			ESP_LOGI(_logTag, "read buffer[%d]: %d", i, _buffer[i]);
					//		}

					// Updating RSSI and SNR
					if (esp_timer_get_time() > _RSSIAndSNRUpdateTimeUs) {
						float valueF;

						if (_SX.getRSSI(valueF) == SX1262Error::none)
							_RSSI = static_cast<int8_t>(valueF);

						if (_SX.getSNR(valueF) == SX1262Error::none)
							_SNR = static_cast<int8_t>(valueF);

						_RSSIAndSNRUpdateTimeUs = esp_timer_get_time() + _RSSIAndSNRUpdateIntervalUs;
					}

					return true;
				}

				logError("receive error", error);

				return false;
			}

			bool beginSpectrumScanning() override {
				// Switching to standby
				const auto error = _SX.setStandby();

				if (error != SX1262Error::none) {
					logError("setSpectrumScanningMode() error", error);
					return false;
				}

				return true;
			}

			bool getSpectrumScanningRecord(const uint32_t frequencyHz, int8_t& RSSI) override {
				// Changing frequency
				auto error = _SX.setRFFrequency(frequencyHz);
				if (error != SX1262Error::none) {
					logError("getSpectrumScanningRSSI() error", error);
					return false;
				}

				vTaskDelay(pdMS_TO_TICKS(10));

				// Moving to RX single mode
				error = _SX.setRX();

				if (error != SX1262Error::none) {
					logError("getSpectrumScanningRSSI() error", error);
					return false;
				}

				// _SX.waitForDIO1Semaphore(10'000);

				// Applying RSSI inst multisampling
				float RSSIF;
				constexpr static uint8_t samplesLength = 32;
				int8_t samples[samplesLength];
				// int64_t samplesSum = 0;

				for (uint8_t i = 0; i < samplesLength; i++) {
					error = _SX.getRSSIInst(RSSIF);

					if (error != SX1262Error::none) {
						logError("getSpectrumScanningRSSI() error", error);
						return false;
					}

					samples[i] = static_cast<int8_t>(RSSIF);
					// samplesSum += static_cast<int8_t>(RSSIF);

					// vTaskDelay(pdMS_TO_TICKS(10));
				}

				// RSSI median value
				std::ranges::sort(samples, std::greater<int8_t>());
				RSSI = samples[samplesLength / 2];
				// RSSI = samplesSum / samplesLength;

				return true;
			}

			bool finishSpectrumScanning() override {
				// Switching to standby
				auto error = _SX.setStandby();

				if (error != SX1262Error::none) {
					logError("setNormalMode() error", error);
					return false;
				}

				// Setting normal RX/TX frequency
				error = _SX.setRFFrequency(config::transceiver::RFFrequencyHz);

				if (error != SX1262Error::none) {
					logError("setNormalMode() error", error);
					return false;
				}

				return true;
			}

		private:
			constexpr static const char* _logTag = "XCVR";

			constexpr static uint8_t _RSSIAndSNRUpdateFrequencyHz = 1;
			constexpr static uint32_t _RSSIAndSNRUpdateIntervalUs = 1'000'000 / _RSSIAndSNRUpdateFrequencyHz;
			int64_t _RSSIAndSNRUpdateTimeUs = 0;

			SX1262 _SX {};
			int8_t _RSSI = 0;
			int8_t _SNR = 0;

			static void logError(const char* key, const SX1262Error error) {
				if (error == SX1262Error::timeout)
					return;

				constexpr static uint8_t errorBufferLength = 255;
				char errorBuffer[errorBufferLength];

				SX1262::errorToString(error, errorBuffer, errorBufferLength);

				ESP_LOGI(_logTag, "%s: %s", key, errorBuffer);
			}
	};
}
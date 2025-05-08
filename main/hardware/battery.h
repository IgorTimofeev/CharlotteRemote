#pragma once

#include <limits>
#include <esp_timer.h>
#include <esp_log.h>
#include "esp_adc/adc_oneshot.h"

namespace pizda {
	class Battery {
		public:
			Battery(
				adc_oneshot_unit_handle_t* adcOneshotUnitHandle,
				adc_channel_t adcChannel,
				uint16_t voltageMin,
				uint16_t voltageMax,
				uint32_t voltageDividerR1,
				uint32_t voltageDividerR2
			) :
				_unitHandle(adcOneshotUnitHandle),
				_channel(adcChannel),
				_voltageMin(voltageMin),
				_voltageMax(voltageMax),
				_voltageDividerR1(voltageDividerR1),
				_voltageDividerR2(voltageDividerR2)
			{

			}

			void setup() const {
				constexpr adc_oneshot_chan_cfg_t channelConfig = {
					.atten = ADC_ATTEN_DB_12,
					.bitwidth = ADC_BITWIDTH_12,
				};

				ESP_ERROR_CHECK(adc_oneshot_config_channel(*_unitHandle, _channel, &channelConfig));
			}

			void tick() {
				int sample;
				ESP_ERROR_CHECK(adc_oneshot_read(*_unitHandle, _channel, &sample));

				_sampleSum += sample;
				_sampleIndex++;

				if (_sampleIndex < _sampleCount)
					return;

				// Reading voltage via ADC instead of cali, because for some reason it gives pretty shitty results
				_voltage = (_sampleSum / _sampleCount) * 3300 / 4096;
				// Restoring battery voltage
				_voltage = _voltage * (_voltageDividerR1 + _voltageDividerR2) / _voltageDividerR2;

//					ESP_LOGI("Battery", "Avg: %lu, mapped: %d", _sampleSum / _sampleCount, _voltage);

				_sampleSum = 0;
				_sampleIndex = 0;
			}

			uint8_t getCharge() const {
				if (_voltage <= _voltageMin) {
					return 0;
				}
				else if (_voltage >= _voltageMax) {
					return 0xFF;
				}
				else {
					return static_cast<uint8_t>((_voltage - _voltageMin) * 0xFF / (_voltageMax - _voltageMin));
				}
			}

			uint16_t getVoltage() const {
				return _voltage;
			}

		private:
			adc_oneshot_unit_handle_t* _unitHandle;
			adc_channel_t _channel;

			uint16_t _voltageMin;
			uint16_t _voltageMax;
			uint32_t _voltageDividerR1;
			uint32_t _voltageDividerR2;

			uint8_t _sampleCount = 8;
			uint32_t _sampleSum = 0;
			uint8_t _sampleIndex = 0;
			uint16_t _voltage = 0;
	};
}
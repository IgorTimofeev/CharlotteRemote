#pragma once

#include <limits>
#include <esp_timer.h>
#include <esp_log.h>
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"

namespace pizda {
	class Battery {
		public:
			Battery(adc_unit_t adcUnit, adc_oneshot_unit_handle_t* adcOneshotUnitHandle, adc_channel_t adcChannel) : _unit(adcUnit), _unitHandle(adcOneshotUnitHandle), _channel(adcChannel) {

			}

			void setup() {
				adc_oneshot_chan_cfg_t channelConfig = {
					.atten = ADC_ATTEN_DB_12,
					.bitwidth = ADC_BITWIDTH_12,
				};

				ESP_ERROR_CHECK(adc_oneshot_config_channel(*_unitHandle, _channel, &channelConfig));

				adc_cali_line_fitting_config_t calibrationConfig = {
					.unit_id = _unit,
					.atten = ADC_ATTEN_DB_12,
					.bitwidth = ADC_BITWIDTH_DEFAULT,
					.default_vref = ADC_CALI_LINE_FITTING_EFUSE_VAL_DEFAULT_VREF
				};

				ESP_ERROR_CHECK(adc_cali_create_scheme_line_fitting(&calibrationConfig, &_caliHandle));
			}

			void tick() {
				const uint32_t time = esp_timer_get_time();

				if (time < _sampleTime)
					return;

				int voltage;
				adc_oneshot_read(*_unitHandle, _channel, &voltage);
				_voltage = voltage;

//				ESP_ERROR_CHECK(adc_oneshot_get_calibrated_result(*_unitHandle, _caliHandle, _channel, &voltage));
//
//				_sampleSum += voltage;
//				_sampleIndex++;
//
//				if (_sampleIndex >= _sampleCount) {
//					_voltage = (_sampleSum / _sampleCount) * _voltageDividerMax / 3300;
//
////					ESP_LOGI("Battery", "Avg: %lu, mapped: %d", _sampleSum / _sampleCount, _voltage);
//
//					_sampleSum = 0;
//					_sampleIndex = 0;
//				}

				_sampleTime = time + _samplingInterval;
			}

			uint16_t getCharge() const {
				if (_voltage <= _voltageDividerMin) {
					return 0;
				}
				else if (_voltage >= _voltageDividerMax) {
					return std::numeric_limits<uint16_t>::max();
				}
				else {
					return (uint32_t) (_voltage - _voltageDividerMin) * std::numeric_limits<uint16_t>::max() / (_voltageDividerMax - _voltageDividerMin);
				}
			}

			uint16_t getVoltage() const {
				return _voltage;
			}

		private:
			static const uint16_t _voltageDividerMin = 1488;
			static const uint16_t _voltageDividerMax = 2084;

			// 8 samples per second should be enough
			static constexpr uint8_t _sampleCount = 8;
			static constexpr uint32_t _samplingInterval = 1000000 / _sampleCount;

			adc_unit_t _unit;
			adc_oneshot_unit_handle_t* _unitHandle;
			adc_channel_t _channel;
			adc_cali_handle_t _caliHandle{};

			uint32_t _sampleTime = 0;
			uint32_t _sampleSum = 0;
			uint8_t _sampleIndex = 0;
			uint16_t _voltage = 0;
	};
}
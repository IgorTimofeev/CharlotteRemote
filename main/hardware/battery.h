#pragma once

#include <limits>
#include <esp_timer.h>
#include <esp_log.h>
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "../constants.h"

namespace pizda {
	class Battery {
		public:
			void setup() {
				adc_oneshot_unit_init_cfg_t unitConfig = {
					.unit_id = ADC_UNIT_1,
					.clk_src = ADC_RTC_CLK_SRC_DEFAULT,
					.ulp_mode = ADC_ULP_MODE_DISABLE
				};

				ESP_ERROR_CHECK(adc_oneshot_new_unit(&unitConfig, &_unitHandle));

				adc_oneshot_chan_cfg_t channelConfig = {
					.atten = ADC_ATTEN_DB_12,
					.bitwidth = ADC_BITWIDTH_12,
				};

				ESP_ERROR_CHECK(adc_oneshot_config_channel(_unitHandle, constants::hardware::battery::channel, &channelConfig));

				adc_cali_line_fitting_config_t calibrationConfig = {
					.unit_id = ADC_UNIT_1,
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
				ESP_ERROR_CHECK(adc_oneshot_get_calibrated_result(_unitHandle, _caliHandle, constants::hardware::battery::channel, &voltage));

				_sampleSum += voltage;
				_sampleIndex++;

				if (_sampleIndex >= _sampleCount) {
					_voltage = (_sampleSum / _sampleIndex) * constants::hardware::battery::voltageDividerMax / 3300;

//				ESP_LOGI("Battery", "ADC: %lu, Mapped: %d", _sampleSum / _sampleIndex, _voltage);

					_sampleSum = 0;
					_sampleIndex = 0;
				}

				_sampleTime = time + _samplingInterval;
			}

			uint16_t getCharge() const {
				if (_voltage <= constants::hardware::battery::voltageDividerMin) {
					return 0;
				}
				else if (_voltage >= constants::hardware::battery::voltageDividerMax) {
					return std::numeric_limits<uint16_t>::max();
				}
				else {
					return (uint32_t) (_voltage - constants::hardware::battery::voltageDividerMin) * std::numeric_limits<uint16_t>::max() / (constants::hardware::battery::voltageDividerMax - constants::hardware::battery::voltageDividerMin);
				}
			}

			uint16_t getVoltage() const {
				return _voltage;
			}

		private:
			// 8 samples per second should be more than enough
			static constexpr uint8_t _sampleCount = 8;
			static constexpr uint32_t _samplingInterval = 1000000 / _sampleCount;

			adc_cali_handle_t _caliHandle{};
			adc_oneshot_unit_handle_t _unitHandle{};

			uint32_t _sampleTime = 0;
			uint32_t _sampleSum = 0;
			uint8_t _sampleIndex = 0;
			uint16_t _voltage = 0;
	};
}
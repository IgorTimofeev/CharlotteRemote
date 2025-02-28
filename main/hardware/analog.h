#pragma once

#include <limits>
#include <esp_timer.h>
#include <esp_log.h>
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"

namespace pizda {
	class Analog {
		public:
			Analog(adc_oneshot_unit_handle_t* adcUnitHandle, adc_channel_t adcChannel) : _channel(adcChannel), _unitHandle(adcUnitHandle) {

			}

			void setup() {
				adc_oneshot_chan_cfg_t channelConfig = {
					.atten = ADC_ATTEN_DB_12,
					.bitwidth = ADC_BITWIDTH_12,
				};

				ESP_ERROR_CHECK(adc_oneshot_config_channel(*_unitHandle, _channel, &channelConfig));
			}

			uint16_t readValue() {
				int value = 0;
				ESP_ERROR_CHECK(adc_oneshot_read(*_unitHandle, _channel, &value));

				return value;
			}

			void tick() {
				const auto newValue = readValue();
				const auto delta = newValue - _value;

				if (std::abs(delta) < 20) {
					return;
				}

				_value = newValue;
			}

			uint16_t getValue() {
				return _value;
			}

			float getFloatValue() {
				return (float) getValue() / 4096.f;
			}

		private:
			adc_channel_t _channel;
			adc_oneshot_unit_handle_t* _unitHandle;
			uint16_t _value = 0;
	};
}
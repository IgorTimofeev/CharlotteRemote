#pragma once

#include <limits>
#include <esp_timer.h>
#include <esp_log.h>
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"

namespace pizda {
	#pragma pack(push, 1)

	class AxisSettings {
		public:
			uint16_t from = 0;
			uint16_t to = 4096;
			bool reverse = false;
	};

	#pragma pack(pop)

	class Axis {
		public:
			Axis(adc_oneshot_unit_handle_t* adcUnitHandle, adc_channel_t adcChannel, AxisSettings* settings) : _channel(adcChannel), _unitHandle(adcUnitHandle), _settings(settings) {

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
				const auto delta = newValue - _rawValue;

				if (std::abs(delta) < 20) {
					return;
				}

				_rawValue = newValue;

				const uint16_t reversed = _settings->reverse ? 4096 - _rawValue : _rawValue;
				const uint16_t clamped = yoba::clamp(reversed, _settings->from, _settings->to);
				const uint16_t deltaFrom = clamped - _settings->from;
				const uint16_t deltaSettings = _settings->to > _settings->from ? _settings->to - _settings->from : 0;

				_processedValue = deltaFrom * 4096 / deltaSettings;
			}

			uint16_t getRawValue() {
				return _rawValue;
			}

			uint16_t getProcessedValue() {
				return _processedValue;
			}

			float getProcessedFloatValue() {
				return (float) getProcessedValue() / 4096.f;
			}

			AxisSettings* getSettings() {
				return _settings;
			}

		private:
			adc_channel_t _channel;
			adc_oneshot_unit_handle_t* _unitHandle;
			uint16_t _rawValue = 0;
			uint16_t _processedValue = 0;
			AxisSettings* _settings;
	};
}
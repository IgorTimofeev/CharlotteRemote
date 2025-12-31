#pragma once

#include <algorithm>

#include <esp_err.h>
#include <driver/ledc.h>

#include "config.h"
#include "sound.h"

namespace pizda {
	class Speaker {
		public:
			void setup() const {
				ledc_timer_config_t timerConfig {};
				timerConfig.speed_mode = config::speaker::mode;
				timerConfig.duty_resolution = static_cast<ledc_timer_bit_t>(_resolutionBits);
				timerConfig.timer_num = config::speaker::timer;
				timerConfig.freq_hz = 4000;
				timerConfig.clk_cfg = LEDC_AUTO_CLK;
				ESP_ERROR_CHECK(ledc_timer_config(&timerConfig));

				ledc_channel_config_t channelConfig {};
				channelConfig.gpio_num = config::speaker::gpio;
				channelConfig.speed_mode = config::speaker::mode;
				channelConfig.channel = config::speaker::channel;
				channelConfig.intr_type = LEDC_INTR_DISABLE;
				channelConfig.timer_sel = config::speaker::timer;
				channelConfig.duty = 0;
				channelConfig.hpoint = 0;
				ESP_ERROR_CHECK(ledc_channel_config(&channelConfig));
			}
			
			static void setState(bool value) {
//				ESP_LOGI("Speaker", "setDuty: %d", value);
				
				ESP_ERROR_CHECK_WITHOUT_ABORT(ledc_set_duty(config::speaker::mode, config::speaker::channel, value ? _dutyMax : 0));
				ESP_ERROR_CHECK_WITHOUT_ABORT(ledc_update_duty(config::speaker::mode, config::speaker::channel));
			}
			
			void setFrequency(uint32_t value) {
//				ESP_LOGI("Speaker", "setFrequency: %d", value);
				
				ESP_ERROR_CHECK_WITHOUT_ABORT(ledc_set_freq(
					config::speaker::mode,
					config::speaker::timer,
					std::clamp<uint32_t>(value, 10, _frequencyMax)
				));
			}

		private:
			constexpr static uint8_t _resolutionBits = 12;
			constexpr static uint32_t _dutyMax = (1 << _resolutionBits) / 2;
			constexpr static uint32_t _frequencyMax = APB_CLK_FREQ / (1 << _resolutionBits);
	};
}
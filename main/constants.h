#pragma once

#include "driver/ledc.h"
#include "cstdint"
#include "esp_adc/adc_oneshot.h"
#include "RadioLib.h"

namespace pizda {
	namespace constants {
		namespace hardware {
			namespace spi {
				constexpr static gpio_num_t mosi = GPIO_NUM_23;
				constexpr static gpio_num_t miso = GPIO_NUM_19;
				constexpr static gpio_num_t sck = GPIO_NUM_18;
			}

			namespace i2c {
				constexpr static gpio_num_t sda = GPIO_NUM_21;
				constexpr static gpio_num_t scl = GPIO_NUM_22;
			}

			namespace screen {
				namespace touch {
					constexpr static gpio_num_t reset = GPIO_NUM_NC;
					constexpr static gpio_num_t interrupt = GPIO_NUM_5;
				}

				constexpr static gpio_num_t slaveSelect = GPIO_NUM_17;
				constexpr static gpio_num_t dataCommand = GPIO_NUM_16;
				constexpr static gpio_num_t reset = GPIO_NUM_NC;
				constexpr static uint32_t frequency = 60000000;
			}

			namespace transceiver {
				constexpr static gpio_num_t slaveSelect = GPIO_NUM_14;
				constexpr static gpio_num_t busy = GPIO_NUM_12;
				constexpr static gpio_num_t dio0 = GPIO_NUM_13;
				constexpr static uint32_t reset = RADIOLIB_NC;
			}

			namespace speaker {
				constexpr static gpio_num_t gpio = GPIO_NUM_4;
				constexpr static ledc_channel_t channel = LEDC_CHANNEL_0;
				constexpr static ledc_timer_t timer = LEDC_TIMER_0;
				constexpr static ledc_mode_t mode = LEDC_LOW_SPEED_MODE;
			}

			namespace hid {

			}
		}

		namespace application {
			constexpr static uint32_t tickInterval = 1000000 / 60;
		}

		namespace transceiver {
			constexpr static uint32_t tickInterval = 1000000 / 60;

			constexpr static uint32_t packetHeader = 0x506C416E;
			constexpr static float frequency = 915;
			constexpr static float bandwidth = 500;
			constexpr static uint8_t spreadingFactor = 7;
			constexpr static uint8_t codingRate = 5;
			constexpr static uint16_t power = 22;
			constexpr static uint16_t preambleLength = 8;
		}

		namespace screen {

		}
	}
}

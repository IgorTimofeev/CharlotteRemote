#pragma once

#include "driver/ledc.h"
#include "cstdint"
#include "esp_adc/adc_oneshot.h"

namespace pizda {
	namespace constants {
		namespace hardware {
			namespace spi {
				static const gpio_num_t mosi = GPIO_NUM_23;
				static const gpio_num_t miso = GPIO_NUM_19;
				static const gpio_num_t sck = GPIO_NUM_18;
			}

			namespace slave {
				static const gpio_num_t slaveSelect = GPIO_NUM_27;
				static const uint32_t frequency = 1000000;
				static const uint32_t tickInterval = 1000000 / 30;
			}

			namespace screen {
				namespace touch {
					static const gpio_num_t reset = GPIO_NUM_32;
					static const gpio_num_t interrupt = GPIO_NUM_34;
					static const gpio_num_t sda = GPIO_NUM_21;
					static const gpio_num_t scl = GPIO_NUM_22;
				}

				static const gpio_num_t slaveSelect = GPIO_NUM_5;
				static const gpio_num_t dataCommand = GPIO_NUM_16;
				static const gpio_num_t reset = GPIO_NUM_17;
				static const uint32_t frequency = 60000000;
			}

			namespace transceiver {
				static const gpio_num_t slaveSelect = GPIO_NUM_4;
				static const gpio_num_t reset = GPIO_NUM_33;
				static const gpio_num_t busy = GPIO_NUM_25;
				static const gpio_num_t dio0 = GPIO_NUM_35;
			}

			namespace speaker {
				static const gpio_num_t gpio = GPIO_NUM_2;
				static const ledc_channel_t channel = LEDC_CHANNEL_0;
				static const ledc_timer_t timer = LEDC_TIMER_0;
				static const ledc_mode_t mode = LEDC_LOW_SPEED_MODE;
			}
		}

		namespace application {
			static const uint32_t tickInterval = 1000000 / 60;
		}

		namespace transceiver {
			static const uint32_t tickInterval = 1000000 / 60;

			static const uint32_t packetHeader = 0x506C416E;
			static const float frequency = 915;
			static const float bandwidth = 500;
			static const uint8_t spreadingFactor = 7;
			static const uint8_t codingRate = 5;
			static const uint16_t power = 22;
			static const uint16_t preambleLength = 8;
		}

		namespace screen {

		}
	}
}

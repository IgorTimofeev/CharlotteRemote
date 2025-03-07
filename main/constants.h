#pragma once

#include "driver/ledc.h"
#include "cstdint"
#include "esp_adc/adc_oneshot.h"
#include "RadioLib.h"

namespace pizda {
	namespace constants {
		namespace spi {
			constexpr static const gpio_num_t mosi = GPIO_NUM_23;
			constexpr static const gpio_num_t miso = GPIO_NUM_19;
			constexpr static const gpio_num_t sck = GPIO_NUM_18;
		}

		namespace i2c {
			constexpr static const gpio_num_t sda = GPIO_NUM_21;
			constexpr static const gpio_num_t scl = GPIO_NUM_22;
		}

		namespace adc {
			static adc_oneshot_unit_handle_t unit1 {};
		}

		namespace wifi {
			constexpr static const char* ssid = "IT";
			constexpr static const char* password = "SERGTIM64ST17";
			constexpr static const uint32_t connectionInterval = 5'000'000;
		}

		namespace ota {
			constexpr static const char* url = "https://github.com/IgorTimofeev/CharlotteDownloads/raw/refs/heads/master/Firmware/Remote.bin";
		}

		namespace screen {
			namespace touch {
				constexpr static const gpio_num_t reset = GPIO_NUM_NC;
				constexpr static const gpio_num_t interrupt = GPIO_NUM_5;
			}

			constexpr static const gpio_num_t slaveSelect = GPIO_NUM_17;
			constexpr static const gpio_num_t dataCommand = GPIO_NUM_16;
			constexpr static const gpio_num_t reset = GPIO_NUM_NC;
			constexpr static const uint32_t frequency = 60000000;
		}

		namespace transceiver {
			constexpr static const uint32_t packetHeader = 0x4348524C;
			constexpr static const uint32_t tickInterval = 1'000'000 / 30;

			namespace lora {
				constexpr static const gpio_num_t slaveSelect = GPIO_NUM_14;
				constexpr static const gpio_num_t busy = GPIO_NUM_12;
				constexpr static const gpio_num_t dio0 = GPIO_NUM_13;
				constexpr static const uint32_t reset = RADIOLIB_NC;

				// Something random, "CHRL" in this case
				constexpr static const float frequency = 915;
				constexpr static const float bandwidth = 500;
				constexpr static const uint8_t spreadingFactor = 7;
				constexpr static const uint8_t codingRate = 5;
				constexpr static const uint16_t power = 22;
				constexpr static const uint16_t preambleLength = 8;
			}

			namespace tcp {
				constexpr static const char* address = "192.168.1.38";
				constexpr static const uint16_t port = 25569;
				constexpr static const uint32_t connectionInterval = 5'000'000;
			}
		}

		namespace axis {
			constexpr static const uint32_t tickInterval = 1'000'000 / 30;

			namespace leverLeft {
				constexpr static adc_oneshot_unit_handle_t* unit = &adc::unit1;
				constexpr static const adc_channel_t channel = ADC_CHANNEL_0;
			}

			namespace leverRight {
				constexpr static adc_oneshot_unit_handle_t* unit = &adc::unit1;
				constexpr static const adc_channel_t channel = ADC_CHANNEL_3;
			}

			namespace joystickHorizontal {
				constexpr static adc_oneshot_unit_handle_t* unit = &adc::unit1;
				constexpr static const adc_channel_t channel = ADC_CHANNEL_7;
			}

			namespace joystickVertical {
				constexpr static adc_oneshot_unit_handle_t* unit = &adc::unit1;
				constexpr static const adc_channel_t channel = ADC_CHANNEL_5;
			}

			namespace ring {
				constexpr static adc_oneshot_unit_handle_t* unit = &adc::unit1;
				constexpr static const adc_channel_t channel = ADC_CHANNEL_6;
			}
		}

		namespace encoder {
			constexpr static const gpio_num_t a = GPIO_NUM_25;
			constexpr static const gpio_num_t b = GPIO_NUM_26;
			constexpr static const gpio_num_t sw = GPIO_NUM_27;
		}

		/**
		Some thoughts about measuring voltage & charge in percents using ADC:

		1) Safe voltage range for Li-ion 18650 battery is [2.5; 4.2]V, and for 2x batteries
		in series it escalates to [5.0; 8.4]V. But let's give it some safety margins like
		[6.0; 8.4]V, because of tons of trash batteries on market

		2) In theory ADC should read up to 3.3V from GPIO, but Espressif docs says that ADC
		configured with 12 dB attenuation can accurately measure only [0.15; 2.45]V on ESP32
		See: https://docs.espressif.com/projects/esp-idf/en/release-v4.3/esp32/api-reference/peripherals/adc.html

		Based on this shit & resistors I have, the voltage divider will be 1M / 330K,
		giving final input range of [1,488; 2.084]V
		*/
		namespace battery {
			constexpr static adc_oneshot_unit_handle_t* unit = &adc::unit1;
			constexpr static const adc_channel_t channel = ADC_CHANNEL_4;

			constexpr static const uint32_t voltageMin = 6000;
			constexpr static const uint32_t voltageMax = 7400;

			constexpr static const uint32_t voltageDividerR1 = 1000000;
			constexpr static const uint32_t voltageDividerR2 = 330000;
		}

		namespace speaker {
			constexpr static const gpio_num_t gpio = GPIO_NUM_4;
			constexpr static const ledc_channel_t channel = LEDC_CHANNEL_0;
			constexpr static const ledc_timer_t timer = LEDC_TIMER_0;
			constexpr static const ledc_mode_t mode = LEDC_LOW_SPEED_MODE;
		}

		namespace application {
			// 30 FPS are barely achievable on SPI displays, so let's take them for max tick rate
			constexpr static const uint32_t mainTickInterval = 1'000'000 / 30;

			constexpr static const uint32_t interpolationTickInterval = 1'000'000 / 30;
		}
	}
}

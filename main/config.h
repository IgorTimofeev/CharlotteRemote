#pragma once

#include <cstdint>

#include <driver/ledc.h>
#include <driver/spi_master.h>
#include <esp_adc/adc_oneshot.h>

#include <SX1262.h>

namespace pizda {
	class config {
		public:
			class spi {
				public:
					constexpr static spi_host_device_t device = SPI2_HOST;
					constexpr static gpio_num_t MOSI = GPIO_NUM_23;
					constexpr static gpio_num_t MISO = GPIO_NUM_19;
					constexpr static gpio_num_t SCK = GPIO_NUM_18;
			};

			class i2c {
				public:
					constexpr static gpio_num_t SDA = GPIO_NUM_21;
					constexpr static gpio_num_t SCL = GPIO_NUM_22;
			};

			class ota {
				public:
					constexpr static const char* url = "https://github.com/IgorTimofeev/CharlotteDownloads/raw/refs/heads/master/Firmware/Remote.bin";
			};

			class screen {
				public:
					class touch {
						public:
							constexpr static gpio_num_t RST = GPIO_NUM_NC;
							constexpr static gpio_num_t INTR = GPIO_NUM_5;
					};

					constexpr static gpio_num_t SS = GPIO_NUM_17;
					constexpr static gpio_num_t DC = GPIO_NUM_16;
					constexpr static gpio_num_t RST = GPIO_NUM_NC;
					constexpr static uint32_t frequency = 60'000'000;
			};
	
			class transceiver {
				public:
					// SX1262 supports up to 16 MHz, but with long wires (10+ cm) there will be troubles, so
					constexpr static uint32_t SPIFrequencyHz = 10'000'000;
					
					constexpr static gpio_num_t SS = GPIO_NUM_14;
					constexpr static gpio_num_t RST = GPIO_NUM_NC;
					constexpr static gpio_num_t busy = GPIO_NUM_12;
					constexpr static gpio_num_t DIO1 = GPIO_NUM_13;

					constexpr static uint32_t RFFrequencyHz = 915'000'000;
					constexpr static auto bandwidth = SX1262::LoRaBandwidth::bw500_0;
					constexpr static uint8_t spreadingFactor = 7;
					constexpr static auto codingRate = SX1262::LoRaCodingRate::cr4_5;
					constexpr static uint8_t syncWord = 0x34;
					constexpr static int8_t powerDBm = 22;
					constexpr static uint16_t preambleLength = 8;
			};
	
			class axes {
				public:
					constexpr static uint8_t tickRateHz = 30;

					class leverLeft {
						public:
							constexpr static adc_unit_t unit = ADC_UNIT_1;
							constexpr static adc_channel_t channel = ADC_CHANNEL_0;
							constexpr static bool invertInput = false;
					};

					class leverRight {
						public:
							constexpr static adc_unit_t unit = ADC_UNIT_1;
							constexpr static adc_channel_t channel = ADC_CHANNEL_3;
							constexpr static bool invertInput = false;
					};

					class joystickHorizontal {
						public:
							constexpr static adc_unit_t unit = ADC_UNIT_1;
							constexpr static adc_channel_t channel = ADC_CHANNEL_7;
							constexpr static bool invertInput = true;
					};

					class joystickVertical {
						public:
							constexpr static adc_unit_t unit = ADC_UNIT_1;
							constexpr static adc_channel_t channel = ADC_CHANNEL_5;
							constexpr static bool invertInput = false;
					};

					class ring {
						public:
							constexpr static adc_unit_t unit = ADC_UNIT_1;
							constexpr static adc_channel_t channel = ADC_CHANNEL_6;
							constexpr static bool invertInput = true;
					};
			};
	
			class encoder {
				public:
					constexpr static gpio_num_t a = GPIO_NUM_25;
					constexpr static gpio_num_t b = GPIO_NUM_26;
					constexpr static gpio_num_t sw = GPIO_NUM_27;
			};
	
			/**
			Some thoughts about measuring voltage & charge in percents using ADC:
	
			1) Safe voltage range for Li-ion 18650 battery is [2.5; 4.2]V, and for 2x batteries
			in series it escalates to [5.0; 8.4]V. But let's give it some safety margins like
			[6.0; 8.4]V, because of tons of trash batteries on market
	
			2) In theory ADC should receive up to 3.3V from GPIO, but Espressif docs says that ADC
			configured with 12 dB attenuation can accurately measure only [0.15; 2.45]V on ESP32
			See: https://docs.espressif.com/projects/esp-idf/en/release-v4.3/esp32/api-reference/peripherals/adc.html
	
			Based on this shit & resistors I have, the voltage divider will be 1M / 330K,
			giving final input range of [1.488; 2.084]V
			*/
			class battery {
				public:
					class remote {
						public:
							constexpr static adc_unit_t unit = ADC_UNIT_1;
							constexpr static adc_channel_t channel = ADC_CHANNEL_4;

							constexpr static uint32_t voltageMin = 2 * 3'000;
							constexpr static uint32_t voltageMax = 2 * 4'200;

							constexpr static uint32_t voltageDividerR1 = 1000'000;
							constexpr static uint32_t voltageDividerR2 = 330'000;
					};

					class aircraft {
						public:
							constexpr static uint32_t voltageMin = 4 * 3'000;
							constexpr static uint32_t voltageMax = 4 * 4'200;
					};
			};
	
			class speaker {
				public:
					constexpr static gpio_num_t gpio = GPIO_NUM_4;
					constexpr static ledc_channel_t channel = LEDC_CHANNEL_0;
					constexpr static ledc_timer_t timer = LEDC_TIMER_0;
					constexpr static ledc_mode_t mode = LEDC_LOW_SPEED_MODE;
			};
	
			class application {
				public:
					// 30 FPS are barely achievable on SPI displays, BUT
					constexpr static uint8_t interfaceTickRateHz = 60;
			};
	};
}

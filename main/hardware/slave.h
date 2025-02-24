#pragma once

#include "../constants.h"

namespace pizda {
	#pragma pack(push, 1)
	struct SlaveData {
		uint16_t leverLeft = 0;
		int32_t encoderRotation = 0;
		uint16_t leverRight = 0;
		uint16_t joystickHorizontal = 0;
		uint16_t joystickVertical = 0;
		uint16_t ring = 0;
		uint8_t batteryCharge = 0;
	};
	#pragma pack(pop)

	class Slave {
		public:
			void setup() {
				// GPIO
				gpio_config_t config = {
					.pin_bit_mask = BIT64(constants::hardware::slave::slaveSelect),
					.mode = GPIO_MODE_OUTPUT,
					.pull_up_en = GPIO_PULLUP_ENABLE,
					.pull_down_en = GPIO_PULLDOWN_DISABLE,
					.intr_type = GPIO_INTR_DISABLE
				};

				gpio_set_level(constants::hardware::slave::slaveSelect, true);

				gpio_config(&config);

				// SPI
				spi_bus_config_t busConfig {};
				busConfig.mosi_io_num = -1;
				busConfig.miso_io_num = constants::hardware::spi::miso;
				busConfig.sclk_io_num = constants::hardware::spi::sck;
				busConfig.quadwp_io_num = -1;
				busConfig.quadhd_io_num = -1;
				busConfig.max_transfer_sz = 0xFFFF;

				ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &busConfig, SPI_DMA_CH_AUTO));

				spi_device_interface_config_t interfaceConfig {};
				interfaceConfig.mode = 0;
				interfaceConfig.clock_speed_hz = (int) constants::hardware::slave::frequency;
				interfaceConfig.spics_io_num = constants::hardware::slave::slaveSelect;
				interfaceConfig.queue_size = 1;

				ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, &interfaceConfig, &_deviceHandle));
			}

			void tick() {
				spi_transaction_t transaction {};
				transaction.length = sizeof(SlaveData)* 8;
				transaction.tx_buffer = nullptr;
				transaction.rx_buffer = &_slaveData;

				ESP_ERROR_CHECK(spi_device_transmit(_deviceHandle, &transaction));
			}

			const SlaveData& getData() const {
				return _slaveData;
			}

		private:
			SlaveData _slaveData {};
			spi_device_handle_t _deviceHandle;
	};
}
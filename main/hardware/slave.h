#pragma once

#include "../constants.h"

namespace pizda {
	#pragma pack(push, 1)
	struct SlaveData {
		uint16_t leverLeft = 0;
		int32_t encoderRotation = 0;
		bool encoderPressed = false;
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

				gpio_config(&config);
				gpio_set_level(constants::hardware::slave::slaveSelect, true);

				// Interface
				spi_device_interface_config_t interfaceConfig {};
				interfaceConfig.mode = 0;
				interfaceConfig.clock_speed_hz = (int) constants::hardware::slave::frequency;
				interfaceConfig.spics_io_num = constants::hardware::slave::slaveSelect;
				interfaceConfig.queue_size = 1;

				ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, &interfaceConfig, &_deviceHandle));
			}

			void tick() {
				if (esp_timer_get_time() < _tickTime)
					return;

				// Transaction
				spi_transaction_t _transaction {};
				_transaction.length = sizeof(SlaveData) * 8;
				_transaction.tx_buffer = nullptr;
				_transaction.rx_buffer = &_slaveData;

				ESP_ERROR_CHECK(spi_device_transmit(_deviceHandle, &_transaction));

//				ESP_LOGI("Data", "------------------------------------");
//
//				ESP_LOGI("Data", "Lever left: %d", _slaveData.leverLeft);
//				ESP_LOGI("Data", "Encoder: %ld", _slaveData.encoderRotation);
//				ESP_LOGI("Data", "Lever right: %d", _slaveData.leverRight);
//
//				ESP_LOGI("Data", "Joy horiz: %d", _slaveData.joystickHorizontal);
//				ESP_LOGI("Data", "Hoy vert: %d", _slaveData.joystickVertical);
//
//				ESP_LOGI("Data", "Ring: %d", _slaveData.ring);
//
//				ESP_LOGI("Data", "BChr: %d", _slaveData.batteryCharge);

				_tickTime = esp_timer_get_time() + constants::hardware::slave::tickInterval;
			}

			const SlaveData& getData() const {
				return _slaveData;
			}

		private:
			uint32_t _tickTime = 0;

			SlaveData _slaveData {};
			spi_device_handle_t _deviceHandle;
	};
}
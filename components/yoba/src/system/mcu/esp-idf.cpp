#include "../../system.h"

#ifdef YOBA_SYSTEM_ESPIDF

#include <cstdio>
#include <cstring>
#include <esp_timer.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "driver/spi_master.h"
#include "driver/i2c_master.h"

namespace yoba::system {
	// -------------------------------- System --------------------------------

	void sleep(uint32_t milliseconds) {
		vTaskDelay(milliseconds / portTICK_PERIOD_MS);
	}

	uint32_t getTime() {
		return (uint32_t) (esp_timer_get_time() / 1000ULL);
	}

	std::unordered_map<uint8_t, std::function<void()>> GPIO::_interruptHandlers = {};

	// -------------------------------- GPIO --------------------------------

	void GPIO::setMode(uint8_t pin, PinMode mode) {
		gpio_config_t conf = {
			.pin_bit_mask = 1ULL << pin,
			.mode = mode == PinMode::Input ? GPIO_MODE_INPUT : GPIO_MODE_OUTPUT,
			.pull_up_en = GPIO_PULLUP_ENABLE,
			.pull_down_en = GPIO_PULLDOWN_DISABLE,
			.intr_type = GPIO_INTR_DISABLE
		};

		gpio_config(&conf);
	}

	bool GPIO::read(uint8_t pin) {
		return gpio_get_level((gpio_num_t) pin);
	}

	void GPIO::write(uint8_t pin, bool value) {
		gpio_set_level((gpio_num_t) pin, value);
	}

	void GPIO::interruptHandler(void *args) {
		auto funcPtr = (std::function<void()>*) args;

		(*funcPtr)();
	}

	void GPIO::addInterruptHandler(uint8_t pin, const std::function<void()>& callback) {
		_interruptHandlers.insert({pin, callback});

		gpio_install_isr_service((int) ESP_INTR_FLAG_IRAM);
		gpio_set_intr_type((gpio_num_t) pin, GPIO_INTR_ANYEDGE);
		gpio_isr_handler_add((gpio_num_t) pin, interruptHandler, (void*) &_interruptHandlers[pin]);
	}

	// -------------------------------- SPI --------------------------------

	spi_device_handle_t SPI::_deviceHandle = spi_device_handle_t();

	// Note: SPI instance will manage slave select output by itself
	void SPI::setup(uint8_t mosiPin, uint8_t sckPin, uint8_t ssPin, uint32_t frequency) {
		// Slave select
		GPIO::setMode(ssPin, system::GPIO::PinMode::Output);
		GPIO::write(ssPin, true);

		spi_bus_config_t busConfig = {
			.mosi_io_num = mosiPin,
			.miso_io_num = -1,
			.sclk_io_num = sckPin,
			.quadwp_io_num = -1,
			.quadhd_io_num = -1,
			.max_transfer_sz = 0xFFFF,
		};

		spi_device_interface_config_t interfaceConfig = {
			.mode = 0,
			.clock_speed_hz = (int) frequency,
			.spics_io_num = ssPin,
			.flags = SPI_DEVICE_NO_DUMMY,
			.queue_size = 1,
		};

		ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &busConfig, SPI_DMA_CH_AUTO));
		ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, &interfaceConfig, &_deviceHandle));
	}

	void SPI::write(uint8_t data) {
		spi_transaction_t transaction;
		memset(&transaction, 0, sizeof(transaction));
		transaction.tx_data[0] = data;
		transaction.flags = SPI_TRANS_USE_TXDATA;
		transaction.length = 8;

		ESP_ERROR_CHECK(spi_device_transmit(_deviceHandle, &transaction));
	}

	void SPI::write(const uint8_t* data, size_t length) {
		spi_transaction_t transaction;
		memset(&transaction, 0, sizeof(transaction));
		transaction.length = length * 8;
		transaction.tx_buffer = data;

		ESP_ERROR_CHECK(spi_device_transmit(_deviceHandle, &transaction));
	}

	// -------------------------------- I2C --------------------------------

	i2c_master_dev_handle_t I2C::_deviceHandle = i2c_master_dev_handle_t();

	void I2C::setup(uint8_t sdaPin, uint8_t sclPin, uint16_t slaveAddress, uint32_t frequency) {
		i2c_master_bus_config_t busConfig = {
			.i2c_port = I2C_NUM_0,
			.sda_io_num = (gpio_num_t) sdaPin,
			.scl_io_num = (gpio_num_t) sclPin,
			.clk_source = I2C_CLK_SRC_DEFAULT,
			.glitch_ignore_cnt = 7,
		};

		i2c_master_bus_handle_t busHandle;

		ESP_ERROR_CHECK(i2c_new_master_bus(&busConfig, &busHandle));

		i2c_device_config_t deviceConfig = {
			.dev_addr_length = I2C_ADDR_BIT_LEN_7,
			.device_address = slaveAddress,
			.scl_speed_hz = frequency,
		};

		ESP_ERROR_CHECK(i2c_master_bus_add_device(busHandle, &deviceConfig, &_deviceHandle));
	}

	void I2C::read(uint8_t* buffer, size_t length) {
		ESP_ERROR_CHECK(i2c_master_receive(_deviceHandle, buffer, length, -1));
	}

	void I2C::write(uint8_t* buffer, size_t length) {
		ESP_ERROR_CHECK(i2c_master_transmit(_deviceHandle, buffer, length, -1));
	}
}

#endif
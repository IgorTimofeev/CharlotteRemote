#pragma once

#include <cstdint>
#include <esp_timer.h>
#include <esp_log.h>
#include "nvs_flash.h"
#include "nvs.h"
#include "hardware/axis.h"

namespace pizda {
	#pragma pack(push, 1)

	class Settings {
		public:
			bool debugInfoVisible = false;

			AxisSettings leverLeftAxis;
			AxisSettings leverRightAxis;
			AxisSettings joystickHorizontalAxis;
			AxisSettings joystickVerticalAxis;
			AxisSettings ringAxis;

			void setup() { // NOLINT(*-convert-member-functions-to-static)
				auto status = nvs_flash_init();

				if (status == ESP_ERR_NVS_NO_FREE_PAGES || status == ESP_ERR_NVS_NEW_VERSION_FOUND) {
					// NVS partition was truncated and needs to be erased
					ESP_ERROR_CHECK(nvs_flash_erase());
					// Retry init
					ESP_ERROR_CHECK(nvs_flash_init());
				}
				else {
					ESP_ERROR_CHECK(status);
				}

				// Opening
				nvs_handle_t handle;
				ESP_ERROR_CHECK(nvs_open("settings", NVS_READWRITE, &handle));

				// Reading version
				uint8_t readVersion = 0;
				status = nvs_get_u8(handle, "version", &readVersion);
				assert(status == ESP_OK || status == ESP_ERR_NVS_NOT_FOUND);

				// Nothing to read || version has changed, need to use default settings
				if (readVersion != _version) {
					ESP_LOGI("Settings", "Found existing, but outdated version: %d", readVersion);

					return;
				}

				// Reading data
				ESP_LOGI("Settings", "Reading existing data");

				size_t settingsSize = sizeof(Settings);
				status = nvs_get_blob(handle, "data", this, &settingsSize);
				assert(status == ESP_OK || status == ESP_ERR_NVS_NOT_FOUND);
			}

			void write() {
				ESP_LOGI("Settings", "Writing");

				// Opening
				nvs_handle_t handle;
				ESP_ERROR_CHECK(nvs_open("settings", NVS_READWRITE, &handle));

				// Writing version & data
				ESP_ERROR_CHECK(nvs_set_u8(handle, "version", _version));
				ESP_ERROR_CHECK(nvs_set_blob(handle, "data", this, sizeof(Settings)));

				// )))
				ESP_ERROR_CHECK(nvs_commit(handle));
			}

			void enqueueWrite() {
				_timeToWrite = esp_timer_get_time() + _writeDelay;
			}

			void tick() {
				if (_timeToWrite == 0 || esp_timer_get_time() < _timeToWrite)
					return;

				_timeToWrite = 0;

				write();
			}

		private:
			static const uint32_t _writeDelay = 2500000;
			static const uint8_t _version = 4;
			uint32_t _timeToWrite = 0;
	};

	#pragma pack(pop)
}

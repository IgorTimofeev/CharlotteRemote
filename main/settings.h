#pragma once

#include <cstdint>
#include <esp_timer.h>
#include <esp_log.h>
#include "nvs_flash.h"
#include "nvs.h"

namespace pizda {
	#pragma pack(push, 1)

	class SettingsAxisData {
		public:
			uint16_t from = 0;
			uint16_t to = 4096;
			bool inverted = false;
	};

	class SettingsAxis {
		public:
			SettingsAxisData leverLeft;
			SettingsAxisData leverRight;
			SettingsAxisData joystickHorizontal;
			SettingsAxisData joystickVertical;
			SettingsAxisData ring;
			uint16_t lowPassFactor = 0xFFFF * 70 / 100;
	};

	class SettingsAutopilot {
		public:
			uint16_t speed;
			uint16_t heading;
			uint16_t altitude;
	};

	class Settings {
		public:
			bool debugInfoVisible = false;
			SettingsAxis axis;
			SettingsAutopilot autopilot;

			void setup() { // NOLINT(*-convert-member-functions-to-static)
				// Opening
				nvs_handle_t handle;
				ESP_ERROR_CHECK(nvs_open("settings", NVS_READWRITE, &handle));

				// Reading version
				uint8_t readVersion = 0;
				auto status = nvs_get_u8(handle, "version", &readVersion);
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
			constexpr static const uint32_t _writeDelay = 2500000;
			constexpr static const uint8_t _version = 2;
			uint32_t _timeToWrite = 0;
	};

	#pragma pack(pop)
}

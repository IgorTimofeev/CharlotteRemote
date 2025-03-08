#pragma once

#include <cstdint>
#include <esp_timer.h>
#include <esp_log.h>
#include "nvs_flash.h"
#include "nvs.h"

#include "types.h"

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
			uint16_t lowPassFactor = 0xFFFF * 75 / 100;
			uint8_t jitteringCutoffValue = 30;
	};

	class SettingsAutopilot {
		public:
			uint16_t speed = 0;
			bool autoThrottle = false;

			uint16_t heading = 0;
			bool headingHold = false;

			uint16_t altitude = 0;
			bool levelChange = false;
	};

	class SettingsControls {
		public:
			uint16_t throttles[2] = {
				0xFFFF,
				0xFFFF
			};

			uint16_t aileronsTrim = 0xFFFF / 2;
			uint16_t elevatorTrim = 0xFFFF / 2;
			uint16_t rudderTrim = 0xFFFF / 2;

			uint32_t referencePressure = 101325;
			AltimeterMode altimeterMode = AltimeterMode::QNH;

			bool landingGear = true;
			bool strobeLights = false;
	};

	class Settings {
		public:
			SettingsAxis axis;
			SettingsControls controls;
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
			constexpr static const uint8_t _version = 3;
			uint32_t _timeToWrite = 0;
	};

	#pragma pack(pop)
}

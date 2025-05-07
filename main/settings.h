#pragma once

#include <cstdint>
#include <esp_timer.h>
#include <esp_log.h>
#include "nvs_flash.h"
#include "nvs.h"
#include "units.h"

namespace pizda {
	#pragma pack(push, 1)

	class SettingsControls {
		public:
			uint8_t throttle = 0x0;

			uint16_t aileronsTrim = 0xFFFF / 2;
			uint16_t elevatorTrim = 0xFFFF / 2;
			uint16_t rudderTrim = 0xFFFF / 2;

			uint32_t referencePressurePa = 101325;
			bool referencePressureSTD = false;

			uint32_t minimumAltitudeFt = 100;
			bool minimumAltitudeEnabled = false;

			bool landingGear = true;
			bool strobeLights = false;
	};

	class SettingsAxisData {
		public:
			uint16_t from = 0;
			uint16_t to = 4096;
			bool inverted = false;
	};

	class SettingsAxis {
		public:
			SettingsAxisData leverLeft {0, 4096, false};
			SettingsAxisData leverRight {0, 4096, false};
			SettingsAxisData joystickHorizontal {778, 2744, true};
			SettingsAxisData joystickVertical {1474, 3031, false};
			SettingsAxisData ring {0, 3768, true};
			uint16_t lowPassFactor = 0xFFFF * 75 / 100;
			uint8_t jitteringCutoffValue = 30;
	};

	class SettingsAutopilot {
		public:
			uint16_t speedKt = 0;
			bool autoThrottle = false;

			uint16_t headingDeg = 0;
			bool headingHold = false;

			uint16_t altitudeFt = 0;
			bool levelChange = false;
	};

	class SettingsUnits {
		public:
			SpeedUnit speed = SpeedUnit::knot;
			DistanceUnit distance = DistanceUnit::foot;
			PressureUnit pressure = PressureUnit::hectopascal;
	};

	class SettingsInterfaceMFDPFD {
		public:
			uint8_t FOV = 60;
			bool flightDirectors = true;
	};

	class SettingsInterfaceMFDND {
		public:
			bool show = false;
			uint8_t heightPercent = 40;
			bool northUp = false;
	};

	enum class SettingsInterfaceMFDInstrumentsMode : uint8_t {
		main,
		autopilot,
		pressure
	};

	class SettingsInterfaceMFD {
		public:
			SettingsInterfaceMFDPFD PFD {};
			SettingsInterfaceMFDND ND {};
			SettingsInterfaceMFDInstrumentsMode instrumentsMode = SettingsInterfaceMFDInstrumentsMode::main;
	};

	class SettingsInterfaceDeveloper {
		public:
			bool debugOverlay = false;
	};

	class SettingsInterface {
		public:
			SettingsInterfaceMFD MFD {};
			SettingsInterfaceDeveloper developer {};
	};

	class Settings {
		public:
			SettingsControls controls {};
			SettingsAxis axis {};
			SettingsAutopilot autopilot {};
			SettingsInterface interface {};
			SettingsUnits units {};

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
			constexpr static uint32_t _writeDelay = 2500000;
			constexpr static uint8_t _version = 15;
			uint32_t _timeToWrite = 0;
	};

	#pragma pack(pop)
}

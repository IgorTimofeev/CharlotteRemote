#pragma once

#include <string.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_ota_ops.h"
#include "esp_http_client.h"
#include "esp_https_ota.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_crt_bundle.h"
#include <functional>

#include "WiFi.h"
#include "../constants.h"

namespace pizda {
	class OTA {
		public:
			void start();
			void stop();
			bool isStarted();

			Callback<uint16_t> onProgressChanged;
			Callback<> onFinished;

		private:
			TaskHandle_t* _task = nullptr;
			esp_https_ota_handle_t _OTAHandle;

			static void ESPEventHandler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);

			static void updateTask(void* arg);
	};
}
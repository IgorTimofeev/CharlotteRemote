#pragma once

#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "packet.h"
#include "../../constants.h"

namespace pizda {
	class WiFiTransceiver {
		public:
			void setup();

			void tick();

		private:
			constexpr static uint8_t _maxRetries = 10;
			static uint8_t _retryCount;

			uint32_t _tickInterval = 0;

			/* FreeRTOS event group to signal when we are connected*/
			static EventGroupHandle_t _WiFiEventGroup;

			static void WiFiEventHandler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);

			void WiFiSetup();
	};
}
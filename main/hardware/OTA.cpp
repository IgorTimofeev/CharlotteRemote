#include "OTA.h"

namespace pizda {
	void OTA::start() {
		if (isStarted())
			return;

		xTaskCreate(updateTask, "OTA task", 8192, this, 1, _task);
	}

	void OTA::stop() {
		if (!isStarted())
			return;

		ESP_LOGE("OTA", "Stopped");

		esp_event_handler_unregister(ESP_HTTPS_OTA_EVENT, ESP_EVENT_ANY_ID, &ESPEventHandler);
		esp_https_ota_abort(_OTAHandle);

		vTaskDelete(*_task);
		_task = nullptr;
	}

	bool OTA::isStarted() {
		return _task != nullptr;
	}

	void OTA::ESPEventHandler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
		if (event_base == ESP_HTTPS_OTA_EVENT) {
			switch (event_id) {
				case ESP_HTTPS_OTA_START:
					ESP_LOGI("OTA", "OTA started");
					break;
				case ESP_HTTPS_OTA_CONNECTED:
					ESP_LOGI("OTA", "Connected to server");
					break;
				case ESP_HTTPS_OTA_GET_IMG_DESC:
					ESP_LOGI("OTA", "Reading Image Description");
					break;
				case ESP_HTTPS_OTA_VERIFY_CHIP_ID:
					ESP_LOGI("OTA", "Verifying chip id of new image: %d", *(esp_chip_id_t *)event_data);
					break;
				case ESP_HTTPS_OTA_DECRYPT_CB:
					ESP_LOGI("OTA", "Callback to decrypt function");
					break;
				case ESP_HTTPS_OTA_WRITE_FLASH:
//							ESP_LOGI("OTA", "Writing to flash: %d written", *(int *)event_data);
					break;
				case ESP_HTTPS_OTA_UPDATE_BOOT_PARTITION:
					ESP_LOGI("OTA", "Boot partition updated. Next Partition: %d", *(esp_partition_subtype_t *)event_data);
					break;
				case ESP_HTTPS_OTA_FINISH:
					ESP_LOGI("OTA", "OTA finish");
					break;
				case ESP_HTTPS_OTA_ABORT:
					ESP_LOGI("OTA", "OTA abort");
					break;
			}
		}
	}

	void OTA::updateTask(void* arg) {
		auto instance = (OTA*) arg;

		ESP_LOGI("OTA", "Starting");

		ESP_ERROR_CHECK(esp_event_handler_register(ESP_HTTPS_OTA_EVENT, ESP_EVENT_ANY_ID, &ESPEventHandler, NULL));

		esp_http_client_config_t httpClientConfig {};
		httpClientConfig.url = constants::ota::url;
		httpClientConfig.timeout_ms = 5'000;
		httpClientConfig.disable_auto_redirect = false;
		httpClientConfig.max_redirection_count = 4;
		httpClientConfig.buffer_size = 2048;
		httpClientConfig.buffer_size_tx = 2048;
		httpClientConfig.crt_bundle_attach = esp_crt_bundle_attach;
		httpClientConfig.keep_alive_enable = true;

		esp_https_ota_config_t OTAConfig {};
		OTAConfig.http_config = &httpClientConfig;

		esp_err_t state = esp_https_ota_begin(&OTAConfig, &instance->_OTAHandle);
		if (state != ESP_OK) {
			ESP_LOGE("OTA", "esp_https_ota_begin() failed");
			return;
		}

		esp_app_desc_t applicationDescription;
		state = esp_https_ota_get_img_desc(instance->_OTAHandle, &applicationDescription);
		if (state != ESP_OK) {
			ESP_LOGE("OTA", "esp_https_ota_get_img_desc() failed");
			instance->stop();
			return;
		}

		// Downloading
		while (true) {
			state = esp_https_ota_perform(instance->_OTAHandle);

			if (state == ESP_ERR_HTTPS_OTA_IN_PROGRESS) {
				const uint16_t progress = (uint16_t) ((uint64_t) esp_https_ota_get_image_len_read(instance->_OTAHandle) * 0xFFFF / (uint64_t) esp_https_ota_get_image_size(instance->_OTAHandle));
				ESP_LOGI("OTA", "Progress: %d", progress * 100 / 0xFFFF);

				instance->onProgressChanged(progress);
			}
			else {
				if (state == ESP_OK) {
					break;
				}
				else {
					ESP_LOGE("OTA", "esp_https_ota_perform() failed");
					instance->stop();
					return;
				}
			}
		}

		if (esp_https_ota_is_complete_data_received(instance->_OTAHandle)) {
			const auto finishState = esp_https_ota_finish(instance->_OTAHandle);

			if (finishState == ESP_OK) {
				ESP_LOGI("OTA", "esp_https_ota_finish() succeeded");

				instance->onFinished();
			}
			else {
				ESP_LOGE("OTA", "esp_https_ota_finish() failed: %d", finishState);

				instance->stop();
			}
		}
		else {
			ESP_LOGE("OTA", "Complete data was not received.");
			instance->stop();
		}
	}
}
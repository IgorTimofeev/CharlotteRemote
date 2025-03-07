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

#include "../constants.h"

namespace pizda {
	class OTA {
		public:
			/* Event handler for catching system events */
			static void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
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
							ESP_LOGI("OTA", "Writing to flash: %d written", *(int *)event_data);
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

			static esp_err_t validate_image_header(esp_app_desc_t *new_app_info)
			{
				if (new_app_info == NULL) {
					return ESP_ERR_INVALID_ARG;
				}

				const esp_partition_t *running = esp_ota_get_running_partition();
				esp_app_desc_t running_app_info;
				if (esp_ota_get_partition_description(running, &running_app_info) == ESP_OK) {
					ESP_LOGI("OTA", "Running firmware version: %s", running_app_info.version);
				}

//				if (memcmp(new_app_info->version, running_app_info.version, sizeof(new_app_info->version)) == 0) {
//					ESP_LOGI("OTA", "Current running version is the same as a new. We will not continue the update.");
//					return ESP_FAIL;
//				}

				return ESP_OK;
			}

			static esp_err_t http_client_init_cb(esp_http_client_handle_t http_client)
			{
				esp_err_t err = ESP_OK;

//				err = esp_http_client_set_header(http_client, "User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/133.0.0.0 Safari/537.36");

				return err;
			}

			static void advanced_ota_example_task()
			{
				ESP_LOGI("OTA", "Starting Advanced OTA example");

				esp_err_t ota_finish_err = ESP_OK;

				esp_http_client_config_t httpClientConfig {};
				httpClientConfig.url = constants::ota::url;
				httpClientConfig.timeout_ms = 5'000;
				httpClientConfig.disable_auto_redirect = false;
				httpClientConfig.max_redirection_count = 8;
				httpClientConfig.buffer_size = 2048;
				httpClientConfig.buffer_size_tx = 2048;
				httpClientConfig.crt_bundle_attach = esp_crt_bundle_attach;
				httpClientConfig.keep_alive_enable = true;


#ifdef CONFIG_EXAMPLE_SKIP_COMMON_NAME_CHECK
				config.skip_cert_common_name_check = true;
#endif

				esp_https_ota_config_t httpsOTAConfig {};
				httpsOTAConfig.http_config = &httpClientConfig;
				// A callback to be invoked after esp_http_client is initialized
				httpsOTAConfig.http_client_init_cb = http_client_init_cb;

				esp_https_ota_handle_t otaHandle = NULL;
				esp_err_t err = esp_https_ota_begin(&httpsOTAConfig, &otaHandle);
				if (err != ESP_OK) {
					ESP_LOGE("OTA", "ESP HTTPS OTA Begin failed");
					return;
				}

				esp_app_desc_t app_desc;
				err = esp_https_ota_get_img_desc(otaHandle, &app_desc);
				if (err != ESP_OK) {
					ESP_LOGE("OTA", "esp_https_ota_get_img_desc failed");
					goto ota_end;
				}

				err = validate_image_header(&app_desc);
				if (err != ESP_OK) {
					ESP_LOGE("OTA", "image header verification failed");
					goto ota_end;
				}

				while (1) {
					err = esp_https_ota_perform(otaHandle);
					if (err != ESP_ERR_HTTPS_OTA_IN_PROGRESS) {
						break;
					}

					// esp_https_ota_perform returns after every read operation which gives user the ability to
					// monitor the status of OTA upgrade by calling esp_https_ota_get_image_len_read, which gives length of image
					// data read so far.
					ESP_LOGI("OTA", "Image bytes read: %d", esp_https_ota_get_image_len_read(otaHandle));
				}

				if (esp_https_ota_is_complete_data_received(otaHandle)) {
					ota_finish_err = esp_https_ota_finish(otaHandle);

					if ((err == ESP_OK) && (ota_finish_err == ESP_OK)) {
						ESP_LOGI("OTA", "ESP_HTTPS_OTA upgrade successful. Rebooting ...");
						vTaskDelay(1000 / portTICK_PERIOD_MS);
						esp_restart();
					}
					else {
						if (ota_finish_err == ESP_ERR_OTA_VALIDATE_FAILED) {
							ESP_LOGE("OTA", "Image validation failed, image is corrupted");
						}
						ESP_LOGE("OTA", "ESP_HTTPS_OTA upgrade failed 0x%x", ota_finish_err);
						return;
					}
				}
				else {
					// the OTA image was not completely received and user can customise the response to this situation.
					ESP_LOGE("OTA", "Complete data was not received.");
				}

				ota_end:
				esp_https_ota_abort(otaHandle);
				ESP_LOGE("OTA", "ESP_HTTPS_OTA upgrade failed");
			}

			void update() {
				// WIFI HERE


				ESP_ERROR_CHECK(esp_event_handler_register(ESP_HTTPS_OTA_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));

				advanced_ota_example_task();
			}

		private:
	};
}
#pragma once

#include <cstdint>
#include <esp_timer.h>
#include <esp_log.h>
#include <memory>
#include <cstring>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_timer.h>
#include <esp_log.h>
#include <bits/locale_facets_nonio.h>

#include "nvs_flash.h"
#include "nvs.h"
#include "units.h"

namespace pizda {
	class NVSController {
		public:
			void openForWriting(const char* key) {
				ESP_ERROR_CHECK(nvs_open(key, NVS_READWRITE, &_handle));
			}

			void openForReading(const char* key) {
				ESP_ERROR_CHECK(nvs_open(key, NVS_READONLY, &_handle));
			}

			void commit() const {
				ESP_ERROR_CHECK(nvs_commit(_handle));
			}

			void close() const {
				nvs_close(_handle);
			}

			uint8_t getUint8(const char* key, uint8_t defaultValue = 0) const {
				return getValue<uint8_t, nvs_get_u8>(key, defaultValue);
			}

			void setUint8(const char* key, uint8_t value) const {
				setValue<uint8_t, nvs_set_u8>(key, value);
			}

			uint16_t getUint16(const char* key, uint16_t defaultValue = 0) const {
				return getValue<uint16_t, nvs_get_u16>(key, defaultValue);
			}

			void setUint16(const char* key, uint16_t value) const {
				setValue<uint16_t, nvs_set_u16>(key, value);
			}

			uint32_t getUint32(const char* key, uint32_t defaultValue = 0) const {
				return getValue<uint32_t, nvs_get_u32>(key, defaultValue);
			}

			void setUint32(const char* key, uint32_t value) const {
				setValue<uint32_t, nvs_set_u32>(key, value);
			}

			float getFloat(const char* key, float defaultValue = 0) const {
				const auto u32 = getUint32(key, defaultValue);

				float result;
				std::memcpy(&result, &u32, sizeof(float));
				return result;
			}

			void setFloat(const char* key, float value) const {
				uint32_t u32;
				std::memcpy(&u32, &value, sizeof(float));
				setUint32(key, u32);
			}

			std::string getString(const char* key, const std::string& defaultValue = std::string()) const {
				return getStringT<char>(key, defaultValue);
			}

			void setString(const char* key, const std::string& value) const {
				setStringT<char>(key, value);
			}

			void testForBullshit() {
				ESP_LOGI("NVS test", "Writing");

				openForWriting("pizda");
				setUint8("uint8Test", 123);
				setUint16("uint16Test", 12345);
				setUint32("uint32Test", 12345);
				setFloat("floatTest", 123.456);
				setString("stringTest", "Pizda penisa");
				commit();
				close();

				ESP_LOGI("NVS test", "Reading");

				openForReading("pizda");
				ESP_LOGI("NVS test", "Value: %d", getUint8("uint8Test"));
				ESP_LOGI("NVS test", "Value: %d", getUint16("uint16Test"));
				ESP_LOGI("NVS test", "Value: %lu", getUint32("uint32Test"));
				ESP_LOGI("NVS test", "Value: %f", getFloat("floatTest"));
				ESP_LOGI("NVS test", "Value: %s", getString("stringTest").c_str());
				close();
			}

		private:
			nvs_handle_t _handle {};

			template<typename TValue, auto Function>
			TValue getValue(const char* key, TValue defaultValue = 0) const {
				TValue result;

				if (Function(_handle, key, &result) != ESP_OK)
					result = defaultValue;

				return result;
			}

			template<typename TValue, auto Function>
			void setValue(const char* key, TValue value) const {
				ESP_ERROR_CHECK(Function(_handle, key, value));
			}

			template<typename TChar>
			std::string getStringT(const char* key, const std::basic_string<TChar>& defaultValue = std::basic_string<TChar>()) const {
				size_t bufferSize = 0;

				if (nvs_get_blob(_handle, key, nullptr, &bufferSize) != ESP_OK)
					return defaultValue;

				uint8_t buffer[bufferSize] {};

				if (nvs_get_blob(_handle, key, &buffer[0], &bufferSize) != ESP_OK)
					return defaultValue;

				return std::string(
					reinterpret_cast<TChar*>(buffer),
					(bufferSize / sizeof(TChar)) - 1
				);
			}

			template<typename TChar>
			void setStringT(const char* key, const std::basic_string<TChar>& value) const {
				ESP_ERROR_CHECK(nvs_set_blob(
					_handle,
					key,
					reinterpret_cast<const uint8_t*>(value.data()),
					(value.size() + 1) * sizeof(char)
				));
			}
	};
}

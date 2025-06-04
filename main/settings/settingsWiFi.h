#pragma once

#include <cstdint>
#include <esp_timer.h>
#include <esp_log.h>
#include <memory>
#include <cstring>
#include <esp_wifi.h>

#include <hardware/NVS/NVSStream.h>
#include <hardware/NVS/NVSSerializable.h>

namespace pizda {
	class SettingsWiFi : public NVSSerializable {
		public:
			bool enabled = true;

		protected:
			const char* getNVSNamespace() override {
				return _namespace;
			}

			void onRead(const NVSStream& stream) override {
				enabled = stream.getBool(_enabled, false);
			}

			void onWrite(const NVSStream& stream) override {
				stream.setBool(_enabled, enabled);
			}

		private:
			constexpr static auto _namespace = "wf";
			constexpr static auto _enabled = "en";
	};
}

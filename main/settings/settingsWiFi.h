#pragma once

#include <cstdint>
#include <esp_timer.h>
#include <esp_log.h>
#include <memory>
#include <cstring>

#include <hardware/NVS/NVSStream.h>
#include <hardware/NVS/NVSSerializable.h>

namespace pizda {
	class SettingsWiFiNetwork {
		public:
			std::string ssid;
			std::string password;
	};

	class SettingsWiFi : public NVSSerializable {
		public:
			bool enabled = true;
			std::optional<SettingsWiFiNetwork> current = std::nullopt;

		protected:
			const char* getNVSNamespace() override {
				return _namespace;
			}

			void onRead(const NVSStream& stream) override {
				enabled = stream.getBool(_enabled, false);

				if (stream.getBool(_has)) {
					const auto ssid = stream.getString(_ssid);
					const auto password = stream.getString(_password);

					current = SettingsWiFiNetwork {
						ssid,
						password
					};
				}
				else {
					// current = std::nullopt;

					current = SettingsWiFiNetwork {
						"IT",
						"SERGTIM64ST17"
					};
				}
			}

			void onWrite(const NVSStream& stream) override {
				stream.setBool(_enabled, enabled);

				if (current.has_value()) {
					stream.setBool(_has, true);
					stream.setString(_ssid, current.value().ssid);
					stream.setString(_password, current.value().password);
				}
				else {
					stream.setBool(_has, false);
				}
			}

		private:
			constexpr static auto _namespace = "wf";
			constexpr static auto _enabled = "en";
			constexpr static auto _has = "hs";
			constexpr static auto _ssid = "ss";
			constexpr static auto _password = "pw";
	};
}

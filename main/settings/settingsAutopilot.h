#pragma once

#include <cstdint>
#include <esp_timer.h>
#include <esp_log.h>
#include <memory>
#include <cstring>

#include <hardware/NVS/NVSStream.h>
#include <hardware/NVS/NVSSerializable.h>

namespace pizda {
	class SettingsAutopilot : public NVSSerializable {
		public:
			uint16_t speedKt = 0;
			bool autoThrottle = false;

			uint16_t headingDeg = 0;
			bool headingHold = false;

			uint16_t altitudeFt = 0;
			bool levelChange = false;

		protected:
			const char* getNVSNamespace() override {
				return _namespace;
			}

			void onRead(const NVSStream& stream) override {
				speedKt = stream.getUint16(_speedKt, 90);
				autoThrottle = stream.getBool(_autoThrottle, false);

				headingDeg = stream.getUint16(_headingDeg, 0);
				headingHold = stream.getBool(_headingHold, false);

				altitudeFt = stream.getUint16(_altitudeFt, 100);
				levelChange = stream.getBool(_levelChange, false);
			}

			void onWrite(const NVSStream& stream) override {
				stream.setUint16(_speedKt, speedKt);
				stream.setBool(_autoThrottle, autoThrottle);

				stream.setUint16(_headingDeg, headingDeg);
				stream.setBool(_headingHold, headingHold);

				stream.setUint16(_altitudeFt, altitudeFt);
				stream.setBool(_levelChange, levelChange);
			}

		private:
			constexpr static auto _namespace = "apns";
			constexpr static auto _speedKt = "apsp";
			constexpr static auto _autoThrottle = "apat";
			constexpr static auto _headingDeg = "aphd";
			constexpr static auto _headingHold = "aphh";
			constexpr static auto _altitudeFt = "apal";
			constexpr static auto _levelChange = "aplc";
	};
}

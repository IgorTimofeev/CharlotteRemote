#pragma once

#include <cstdint>
#include <esp_timer.h>
#include <esp_log.h>
#include <memory>
#include <cstring>

#include <NVSSettings.h>

namespace pizda {
	using namespace YOBA;
	
	class SettingsAutopilot : public NVSSettings {
		public:
			uint16_t speedKt = 0;
			bool autoThrottle = false;

			uint16_t headingDeg = 0;
			bool headingHold = false;

			uint16_t altitudeFt = 0;
			bool levelChange = false;

		protected:
			const char* getNamespace() override {
				return _namespace;
			}

			void onRead(const NVSStream& stream) override {
				speedKt = stream.readUint16(_speedKt, 90);
				autoThrottle = stream.readBool(_autoThrottle, false);

				headingDeg = stream.readUint16(_headingDeg, 0);
				headingHold = stream.readBool(_headingHold, false);

				altitudeFt = stream.readUint16(_altitudeFt, 100);
				levelChange = stream.readBool(_levelChange, false);
			}

			void onWrite(const NVSStream& stream) override {
				stream.writeUint16(_speedKt, speedKt);
				stream.writeBool(_autoThrottle, autoThrottle);

				stream.writeUint16(_headingDeg, headingDeg);
				stream.writeBool(_headingHold, headingHold);

				stream.writeUint16(_altitudeFt, altitudeFt);
				stream.writeBool(_levelChange, levelChange);
			}

		private:
			constexpr static auto _namespace = "ap";
			constexpr static auto _speedKt = "sp";
			constexpr static auto _autoThrottle = "at";
			constexpr static auto _headingDeg = "hd";
			constexpr static auto _headingHold = "hh";
			constexpr static auto _altitudeFt = "al";
			constexpr static auto _levelChange = "lc";
	};
}

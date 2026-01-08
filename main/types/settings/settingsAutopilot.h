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
			uint16_t headingDeg = 0;
			uint16_t altitudeFt = 0;

		protected:
			const char* getNamespace() override {
				return _namespace;
			}

			void onRead(const NVSStream& stream) override {
				speedKt = stream.readUint16(_speedKt, 90);

				headingDeg = stream.readUint16(_headingDeg, 0);

				altitudeFt = stream.readUint16(_altitudeFt, 100);
			}

			void onWrite(const NVSStream& stream) override {
				stream.writeUint16(_speedKt, speedKt);

				stream.writeUint16(_headingDeg, headingDeg);

				stream.writeUint16(_altitudeFt, altitudeFt);
			}

		private:
			constexpr static auto _namespace = "ap";
			constexpr static auto _speedKt = "sp";
			constexpr static auto _headingDeg = "hd";
			constexpr static auto _altitudeFt = "al";
	};
}

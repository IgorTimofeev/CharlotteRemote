#pragma once

#include <cstdint>
#include <esp_timer.h>
#include <esp_log.h>
#include <memory>
#include <cstring>

#include <hardware/NVS/NVSStream.h>
#include <hardware/NVS/NVSSerializable.h>

namespace pizda {
	class SettingsControls : public NVSSerializable {
		public:
			uint8_t throttle = 0;

			uint32_t referencePressurePa = 0;
			bool referencePressureSTD = false;

			uint32_t minimumAltitudeFt = 0;
			bool minimumAltitudeEnabled = false;

			bool landingGear = false;
			bool strobeLights = false;

		protected:
			const char* getNVSNamespace() override {
				return _namespace;
			}

			void onRead(const NVSStream& stream) override {
				throttle = stream.getFloat(_throttle, 0);

				referencePressurePa = stream.getUint32(_referencePressurePa, 101325);
				referencePressureSTD = stream.getBool(_referencePressureSTD, false);

				minimumAltitudeFt = stream.getUint32(_minimumAltitudeFt, 350);
				minimumAltitudeEnabled = stream.getBool(_minimumAltitudeEnabled, true);

				landingGear = stream.getBool(_landingGear, true);
				strobeLights = stream.getBool(_strobeLights, true);
			}

			void onWrite(const NVSStream& stream) override {
				stream.setFloat(_throttle, throttle);

				stream.setUint32(_referencePressurePa, referencePressurePa);
				stream.setBool(_referencePressureSTD, referencePressureSTD);

				stream.setUint32(_minimumAltitudeFt, minimumAltitudeFt);
				stream.setBool(_minimumAltitudeEnabled, minimumAltitudeEnabled);

				stream.setBool(_landingGear, landingGear);
				stream.setBool(_strobeLights, strobeLights);
			}

		private:
			constexpr static auto _namespace = "ct";
			constexpr static auto _throttle = "th";
			constexpr static auto _referencePressurePa = "rp";
			constexpr static auto _referencePressureSTD = "rs";
			constexpr static auto _minimumAltitudeFt = "ma";
			constexpr static auto _minimumAltitudeEnabled = "me";
			constexpr static auto _landingGear = "lg";
			constexpr static auto _strobeLights = "sl";
	};
}

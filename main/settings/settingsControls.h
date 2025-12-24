#pragma once

#include <cstdint>
#include <esp_timer.h>
#include <esp_log.h>
#include <memory>
#include <cstring>

#include <NVSSettings.h>

namespace pizda {
	using namespace YOBA;
	
	class SettingsControls : public NVSSettings {
		public:
			uint8_t throttle = 0;

			uint32_t referencePressurePa = 0;
			bool referencePressureSTD = false;

			uint32_t minimumAltitudeFt = 0;
			bool minimumAltitudeEnabled = false;

			bool landingGear = false;
			bool strobeLights = false;

		protected:
			const char* getNamespace() override {
				return _namespace;
			}

			void onRead(const NVSStream& stream) override {
				throttle = stream.readUint8(_throttle, 0);

				referencePressurePa = stream.readUint32(_referencePressurePa, 101325);
				referencePressureSTD = stream.readBool(_referencePressureSTD, false);

				minimumAltitudeFt = stream.readUint32(_minimumAltitudeFt, 350);
				minimumAltitudeEnabled = stream.readBool(_minimumAltitudeEnabled, true);

				landingGear = stream.readBool(_landingGear, true);
				strobeLights = stream.readBool(_strobeLights, true);
			}

			void onWrite(const NVSStream& stream) override {
				stream.writeUint8(_throttle, throttle);

				stream.writeUint32(_referencePressurePa, referencePressurePa);
				stream.writeBool(_referencePressureSTD, referencePressureSTD);

				stream.writeUint32(_minimumAltitudeFt, minimumAltitudeFt);
				stream.writeBool(_minimumAltitudeEnabled, minimumAltitudeEnabled);

				stream.writeBool(_landingGear, landingGear);
				stream.writeBool(_strobeLights, strobeLights);
			}

		private:
			constexpr static const char* _namespace = "ct2";
			constexpr static const char* _throttle = "th";
			constexpr static const char* _referencePressurePa = "rp";
			constexpr static const char* _referencePressureSTD = "rs";
			constexpr static const char* _minimumAltitudeFt = "ma";
			constexpr static const char* _minimumAltitudeEnabled = "me";
			constexpr static const char* _landingGear = "lg";
			constexpr static const char* _strobeLights = "sl";
	};
}

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
			uint32_t referencePressurePa = 0;
			bool referencePressureSTD = false;

			uint32_t minimumAltitudeFt = 0;
			bool minimumAltitudeEnabled = false;
			
			// Range is [-100; 100]
			int16_t aileronsTrim {};
			int16_t elevatorTrim {};
			int16_t rudderTrim {};

		protected:
			const char* getNamespace() override {
				return _namespace;
			}

			void onRead(const NVSStream& stream) override {
				referencePressurePa = stream.readUint32(_referencePressurePa, 101325);
				referencePressureSTD = stream.readBool(_referencePressureSTD, false);

				minimumAltitudeFt = stream.readUint32(_minimumAltitudeFt, 350);
				minimumAltitudeEnabled = stream.readBool(_minimumAltitudeEnabled, true);
				
				aileronsTrim = stream.readInt16(_aileronsTrim, 0);
				elevatorTrim = stream.readInt16(_elevatorTrim, 0);
				rudderTrim = stream.readInt16(_rudderTrim, 0);
			}

			void onWrite(const NVSStream& stream) override {
				stream.writeUint32(_referencePressurePa, referencePressurePa);
				stream.writeBool(_referencePressureSTD, referencePressureSTD);

				stream.writeUint32(_minimumAltitudeFt, minimumAltitudeFt);
				stream.writeBool(_minimumAltitudeEnabled, minimumAltitudeEnabled);
				
				stream.writeInt16(_aileronsTrim, aileronsTrim);
				stream.writeInt16(_elevatorTrim, elevatorTrim);
				stream.writeInt16(_rudderTrim, rudderTrim);
			}

		private:
			constexpr static const char* _namespace = "ct2";
			
			constexpr static const char* _referencePressurePa = "rp";
			constexpr static const char* _referencePressureSTD = "rs";
			
			constexpr static const char* _minimumAltitudeFt = "ma";
			constexpr static const char* _minimumAltitudeEnabled = "me";
			
			constexpr static const char* _aileronsTrim = "ta";
			constexpr static const char* _elevatorTrim = "te";
			constexpr static const char* _rudderTrim = "tr";
			
	};
}

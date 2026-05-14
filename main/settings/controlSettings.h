#pragma once

#include <NVSSettings.h>

namespace pizda {
	using namespace YOBA;
	
	class ControlSettings : public NVSSettings {
		public:
			// Range is [-100; 100]
			int8_t aileronsTrim {};
			int8_t elevatorTrim {};
			int8_t rudderTrim {};

			int16_t cameraPitchDeg = 0;
			int16_t cameraYawDeg = 0;

		protected:
			const char* getNamespace() override {
				return _namespace;
			}

			void onRead(const NVSStream& stream) override {
				aileronsTrim = stream.readInt8(_aileronsTrim, 0);
				elevatorTrim = stream.readInt8(_elevatorTrim, 0);
				rudderTrim = stream.readInt8(_rudderTrim, 0);

				cameraPitchDeg = stream.readInt16(_cameraPitch, 0);
				cameraYawDeg = stream.readInt16(_cameraYaw, 0);
			}

			void onWrite(const NVSStream& stream) override {
				stream.writeInt8(_aileronsTrim, aileronsTrim);
				stream.writeInt8(_elevatorTrim, elevatorTrim);
				stream.writeInt8(_rudderTrim, rudderTrim);

				stream.writeInt16(_cameraPitch, cameraPitchDeg);
				stream.writeInt16(_cameraYaw, cameraYawDeg);
			}

		private:
			constexpr static auto _namespace = "ct4";
			
			constexpr static auto _aileronsTrim = "ta";
			constexpr static auto _elevatorTrim = "te";
			constexpr static auto _rudderTrim = "tr";

			constexpr static auto _cameraPitch = "cp";
			constexpr static auto _cameraYaw = "cy";

	};
}

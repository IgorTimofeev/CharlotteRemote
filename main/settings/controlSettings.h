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

		protected:
			const char* getNamespace() override {
				return _namespace;
			}

			void onRead(const NVSStream& stream) override {
				aileronsTrim = stream.readInt8(_aileronsTrim, 0);
				elevatorTrim = stream.readInt8(_elevatorTrim, 0);
				rudderTrim = stream.readInt8(_rudderTrim, 0);
			}

			void onWrite(const NVSStream& stream) override {
				stream.writeInt16(_aileronsTrim, aileronsTrim);
				stream.writeInt16(_elevatorTrim, elevatorTrim);
				stream.writeInt16(_rudderTrim, rudderTrim);
			}

		private:
			constexpr static const char* _namespace = "ct3";
			
			constexpr static const char* _aileronsTrim = "ta";
			constexpr static const char* _elevatorTrim = "te";
			constexpr static const char* _rudderTrim = "tr";
			
	};
}

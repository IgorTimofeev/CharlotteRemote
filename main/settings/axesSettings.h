#pragma once

#include <cstdint>
#include <esp_timer.h>
#include <esp_log.h>
#include <memory>
#include <cstring>

#include <NVSSettings.h>

namespace pizda {
	using namespace YOBA;
	
	class AxisSettingsData {
		public:
			uint16_t from = 0;
			uint16_t to = 0;
			uint8_t sensitivity = 0;
	};

	class AxesSettings: public NVSSettings {
		public:
			AxisSettingsData leverLeft {};
			AxisSettingsData leverRight {};
			AxisSettingsData joystickHorizontal  {};
			AxisSettingsData joystickVertical {};
			AxisSettingsData ring {};
			uint16_t lowPassFactor = 0;
			uint8_t jitteringCutoffValue = 0;

		protected:
			const char* getNamespace() override {
				return _namespace;
			}

			void onRead(const NVSStream& stream) override {
				leverLeft.from = stream.readUint16(_leverLeftFrom, 0);
				leverLeft.to = stream.readUint16(_leverLeftTo, 4096);
				leverLeft.sensitivity = stream.readUint8(_leverLeftSensitivity, 0);

				leverRight.from = stream.readUint16(_leverRightFrom, 0);
				leverRight.to = stream.readUint16(_leverRightTo, 4096);
				leverRight.sensitivity = stream.readUint8(_leverRightSensitivity, 0);

				joystickHorizontal.from = stream.readUint16(_joystickHorizontalFrom, 778);
				joystickHorizontal.to = stream.readUint16(_joystickHorizontalTo, 2744);
				joystickHorizontal.sensitivity = stream.readUint8(_joystickHorizontalSensitivity, 0);

				joystickVertical.from = stream.readUint16(_joystickVerticalFrom, 1474);
				joystickVertical.to = stream.readUint16(_joystickVerticalTo, 3031);
				joystickVertical.sensitivity = stream.readUint8(_joystickVerticalSensitivity, 0);

				ring.from = stream.readUint16(_ringFrom, 0);
				ring.to = stream.readUint16(_ringTo, 3768);
				ring.sensitivity = stream.readUint8(_ringSensitivity, 0x00);

				lowPassFactor = stream.readUint16(_lowPassFactor, 0xFFFF * 75 / 100);
				jitteringCutoffValue = stream.readUint8(_jitteringCutoffValue, 30);
			}

			void onWrite(const NVSStream& stream) override {
				stream.writeUint16(_leverLeftFrom, leverLeft.from);
				stream.writeUint16(_leverLeftTo, leverLeft.to);
				stream.writeUint8(_leverLeftSensitivity, leverLeft.sensitivity);

				stream.writeUint16(_leverRightFrom, leverRight.from);
				stream.writeUint16(_leverRightTo, leverRight.to);
				stream.writeUint8(_leverRightSensitivity, leverRight.sensitivity);

				stream.writeUint16(_joystickHorizontalFrom, joystickHorizontal.from);
				stream.writeUint16(_joystickHorizontalTo, joystickHorizontal.to);
				stream.writeUint8(_joystickHorizontalSensitivity, joystickHorizontal.sensitivity);

				stream.writeUint16(_joystickVerticalFrom, joystickVertical.from);
				stream.writeUint16(_joystickVerticalTo, joystickVertical.to);
				stream.writeUint8(_joystickVerticalSensitivity, joystickVertical.sensitivity);

				stream.writeUint16(_ringFrom, ring.from);
				stream.writeUint16(_ringTo, ring.to);
				stream.writeUint8(_ringSensitivity, ring.sensitivity);

				stream.writeUint16(_lowPassFactor, lowPassFactor);
				stream.writeUint8(_jitteringCutoffValue, jitteringCutoffValue);
			}

		private:
			constexpr static auto _namespace = "ax2";
			
			constexpr static auto _leverLeftFrom = "llf";
			constexpr static auto _leverLeftTo = "llt";
			constexpr static auto _leverLeftSensitivity = "lls";

			constexpr static auto _leverRightFrom = "lrf";
			constexpr static auto _leverRightTo = "lrt";
			constexpr static auto _leverRightSensitivity = "lrs";

			constexpr static auto _joystickHorizontalFrom = "jhf";
			constexpr static auto _joystickHorizontalTo = "jht";
			constexpr static auto _joystickHorizontalSensitivity = "jhs";

			constexpr static auto _joystickVerticalFrom = "jvf";
			constexpr static auto _joystickVerticalTo = "jvt";
			constexpr static auto _joystickVerticalSensitivity = "jvs";

			constexpr static auto _ringFrom = "rgf";
			constexpr static auto _ringTo = "rgt";
			constexpr static auto _ringSensitivity = "rgs";

			constexpr static auto _lowPassFactor = "lpf";
			constexpr static auto _jitteringCutoffValue = "jc";
	};
}

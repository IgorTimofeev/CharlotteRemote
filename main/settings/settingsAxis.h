#pragma once

#include <cstdint>
#include <esp_timer.h>
#include <esp_log.h>
#include <memory>
#include <cstring>

#include <hardware/NVS/NVSStream.h>
#include <hardware/NVS/NVSSerializable.h>

namespace pizda {
	class SettingsAxisData {
		public:
			uint16_t from = 0;
			uint16_t to = 0;
			bool inverted = false;
	};

	class SettingsAxis: public NVSSerializable {
		public:
			SettingsAxisData leverLeft {};
			SettingsAxisData leverRight {};
			SettingsAxisData joystickHorizontal  {};
			SettingsAxisData joystickVertical {};
			SettingsAxisData ring {};
			uint16_t lowPassFactor = 0;
			uint8_t jitteringCutoffValue = 0;

		protected:
			const char* getNVSNamespace() override {
				return _namespace;
			}

			void onRead(const NVSStream& stream) override {
				leverLeft.from = stream.getUint16(_leverLeftFrom, 0);
				leverLeft.to = stream.getUint16(_leverLeftTo, 4096);
				leverLeft.inverted = stream.getUint16(_leverLeftInverted, false);

				leverRight.from = stream.getUint16(_leverRightFrom, 0);
				leverRight.to = stream.getUint16(_leverRightTo, 4096);
				leverRight.inverted = stream.getUint16(_leverRightInverted, false);

				joystickHorizontal.from = stream.getUint16(_joystickHorizontalFrom, 778);
				joystickHorizontal.to = stream.getUint16(_joystickHorizontalTo, 2744);
				joystickHorizontal.inverted = stream.getUint16(_joystickHorizontalInverted, true);

				joystickVertical.from = stream.getUint16(_joystickVerticalFrom, 1474);
				joystickVertical.to = stream.getUint16(_joystickVerticalTo, 3031);
				joystickVertical.inverted = stream.getUint16(_joystickVerticalInverted, false);

				ring.from = stream.getUint16(_ringFrom, 0);
				ring.to = stream.getUint16(_ringTo, 3768);
				ring.inverted = stream.getUint16(_ringInverted, true);

				lowPassFactor = stream.getUint16(_lowPassFactor, 0xFFFF * 75 / 100);
				jitteringCutoffValue = stream.getUint8(_jitteringCutoffValue, 30);
			}

			void onWrite(const NVSStream& stream) override {
				stream.setUint16(_leverLeftFrom, leverLeft.from);
				stream.setUint16(_leverLeftTo, leverLeft.to);
				stream.setUint16(_leverLeftInverted, leverLeft.inverted);

				stream.setUint16(_leverRightFrom, leverRight.from);
				stream.setUint16(_leverRightTo, leverRight.to);
				stream.setUint16(_leverRightInverted, leverRight.inverted);

				stream.setUint16(_joystickHorizontalFrom, joystickHorizontal.from);
				stream.setUint16(_joystickHorizontalTo, joystickHorizontal.to);
				stream.setUint16(_joystickHorizontalInverted, joystickHorizontal.inverted);

				stream.setUint16(_joystickVerticalFrom, joystickVertical.from);
				stream.setUint16(_joystickVerticalTo, joystickVertical.to);
				stream.setUint16(_joystickVerticalInverted, joystickVertical.inverted);

				stream.setUint16(_ringFrom, ring.from);
				stream.setUint16(_ringTo, ring.to);
				stream.setUint16(_ringInverted, ring.inverted);

				stream.setUint16(_lowPassFactor, lowPassFactor);
				stream.setUint8(_jitteringCutoffValue, jitteringCutoffValue);
			}

		private:
			constexpr static auto _namespace = "ax";
			constexpr static auto _leverLeftFrom = "llf";
			constexpr static auto _leverLeftTo = "llt";
			constexpr static auto _leverLeftInverted = "lli";
			constexpr static auto _leverRightFrom = "lrf";
			constexpr static auto _leverRightTo = "lrt";
			constexpr static auto _leverRightInverted = "lri";
			constexpr static auto _joystickHorizontalFrom = "jhf";
			constexpr static auto _joystickHorizontalTo = "jht";
			constexpr static auto _joystickHorizontalInverted = "jhi";
			constexpr static auto _joystickVerticalFrom = "jvf";
			constexpr static auto _joystickVerticalTo = "jvt";
			constexpr static auto _joystickVerticalInverted = "jvi";
			constexpr static auto _ringFrom = "rgf";
			constexpr static auto _ringTo = "rgt";
			constexpr static auto _ringInverted = "rgi";
			constexpr static auto _lowPassFactor = "lpf";
			constexpr static auto _jitteringCutoffValue = "jc";
	};
}

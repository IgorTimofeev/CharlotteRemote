#pragma once

#include <cstdint>
#include <esp_timer.h>
#include <esp_log.h>
#include <memory>
#include <cstring>

#include <NVSSettings.h>

namespace pizda {
	using namespace YOBA;
	
	class SettingsAxisData {
		public:
			uint16_t from = 0;
			uint16_t to = 0;
			bool inverted = false;
	};

	class SettingsAxis: public NVSSettings {
		public:
			SettingsAxisData leverLeft {};
			SettingsAxisData leverRight {};
			SettingsAxisData joystickHorizontal  {};
			SettingsAxisData joystickVertical {};
			SettingsAxisData ring {};
			uint16_t lowPassFactor = 0;
			uint8_t jitteringCutoffValue = 0;

		protected:
			const char* getNamespace() override {
				return _namespace;
			}

			void onRead(const NVSStream& stream) override {
				leverLeft.from = stream.readUint16(_leverLeftFrom, 0);
				leverLeft.to = stream.readUint16(_leverLeftTo, 4096);
				leverLeft.inverted = stream.readUint16(_leverLeftInverted, false);

				leverRight.from = stream.readUint16(_leverRightFrom, 0);
				leverRight.to = stream.readUint16(_leverRightTo, 4096);
				leverRight.inverted = stream.readUint16(_leverRightInverted, false);

				joystickHorizontal.from = stream.readUint16(_joystickHorizontalFrom, 778);
				joystickHorizontal.to = stream.readUint16(_joystickHorizontalTo, 2744);
				joystickHorizontal.inverted = stream.readUint16(_joystickHorizontalInverted, true);

				joystickVertical.from = stream.readUint16(_joystickVerticalFrom, 1474);
				joystickVertical.to = stream.readUint16(_joystickVerticalTo, 3031);
				joystickVertical.inverted = stream.readUint16(_joystickVerticalInverted, false);

				ring.from = stream.readUint16(_ringFrom, 0);
				ring.to = stream.readUint16(_ringTo, 3768);
				ring.inverted = stream.readUint16(_ringInverted, true);

				lowPassFactor = stream.readUint16(_lowPassFactor, 0xFFFF * 75 / 100);
				jitteringCutoffValue = stream.readUint8(_jitteringCutoffValue, 30);
			}

			void onWrite(const NVSStream& stream) override {
				stream.writeUint16(_leverLeftFrom, leverLeft.from);
				stream.writeUint16(_leverLeftTo, leverLeft.to);
				stream.writeUint16(_leverLeftInverted, leverLeft.inverted);

				stream.writeUint16(_leverRightFrom, leverRight.from);
				stream.writeUint16(_leverRightTo, leverRight.to);
				stream.writeUint16(_leverRightInverted, leverRight.inverted);

				stream.writeUint16(_joystickHorizontalFrom, joystickHorizontal.from);
				stream.writeUint16(_joystickHorizontalTo, joystickHorizontal.to);
				stream.writeUint16(_joystickHorizontalInverted, joystickHorizontal.inverted);

				stream.writeUint16(_joystickVerticalFrom, joystickVertical.from);
				stream.writeUint16(_joystickVerticalTo, joystickVertical.to);
				stream.writeUint16(_joystickVerticalInverted, joystickVertical.inverted);

				stream.writeUint16(_ringFrom, ring.from);
				stream.writeUint16(_ringTo, ring.to);
				stream.writeUint16(_ringInverted, ring.inverted);

				stream.writeUint16(_lowPassFactor, lowPassFactor);
				stream.writeUint8(_jitteringCutoffValue, jitteringCutoffValue);
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

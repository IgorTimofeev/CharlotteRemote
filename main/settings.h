#pragma once

#include <cstdint>
#include <esp_timer.h>
#include <esp_log.h>
#include <memory>
#include <cstring>

#include "hardware/NVS/NVSStream.h"
#include "hardware/NVS/NVSSerializable.h"

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
			constexpr static auto _namespace = "ctns";
			constexpr static auto _throttle = "ctth";
			constexpr static auto _referencePressurePa = "ctrp";
			constexpr static auto _referencePressureSTD = "ctrs";
			constexpr static auto _minimumAltitudeFt = "ctma";
			constexpr static auto _minimumAltitudeEnabled = "ctme";
			constexpr static auto _landingGear = "ctlg";
			constexpr static auto _strobeLights = "ctsl";
	};

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
			constexpr static auto _namespace = "axns";
			constexpr static auto _leverLeftFrom = "axllf";
			constexpr static auto _leverLeftTo = "axllt";
			constexpr static auto _leverLeftInverted = "axlli";
			constexpr static auto _leverRightFrom = "axlrf";
			constexpr static auto _leverRightTo = "axlrt";
			constexpr static auto _leverRightInverted = "axlri";
			constexpr static auto _joystickHorizontalFrom = "axjhf";
			constexpr static auto _joystickHorizontalTo = "axjht";
			constexpr static auto _joystickHorizontalInverted = "axjhi";
			constexpr static auto _joystickVerticalFrom = "axjvf";
			constexpr static auto _joystickVerticalTo = "axjvt";
			constexpr static auto _joystickVerticalInverted = "axjvi";
			constexpr static auto _ringFrom = "axrgf";
			constexpr static auto _ringTo = "axrgt";
			constexpr static auto _ringInverted = "axrgi";
			constexpr static auto _lowPassFactor = "axlpf";
			constexpr static auto _jitteringCutoffValue = "axjc";
	};

	class SettingsAutopilot : public NVSSerializable {
		public:
			uint16_t speedKt = 0;
			bool autoThrottle = false;

			uint16_t headingDeg = 0;
			bool headingHold = false;

			uint16_t altitudeFt = 0;
			bool levelChange = false;

		protected:
			const char* getNVSNamespace() override {
				return _namespace;
			}

			void onRead(const NVSStream& stream) override {
				speedKt = stream.getUint16(_speedKt, 90);
				autoThrottle = stream.getBool(_autoThrottle, false);

				headingDeg = stream.getUint16(_headingDeg, 0);
				headingHold = stream.getBool(_headingHold, false);

				altitudeFt = stream.getUint16(_altitudeFt, 100);
				levelChange = stream.getBool(_levelChange, false);
			}

			void onWrite(const NVSStream& stream) override {
				stream.setUint16(_speedKt, speedKt);
				stream.setBool(_autoThrottle, autoThrottle);

				stream.setUint16(_headingDeg, headingDeg);
				stream.setBool(_headingHold, headingHold);

				stream.setUint16(_altitudeFt, altitudeFt);
				stream.setBool(_levelChange, levelChange);
			}

		private:
			constexpr static auto _namespace = "apns";
			constexpr static auto _speedKt = "apsp";
			constexpr static auto _autoThrottle = "apat";
			constexpr static auto _headingDeg = "aphd";
			constexpr static auto _headingHold = "aphh";
			constexpr static auto _altitudeFt = "apal";
			constexpr static auto _levelChange = "aplc";
	};

	class SettingsInterfaceMFDPFD {
		public:
			bool visible = false;
			uint8_t FOV = 0;
			bool flightDirectors = false;
	};

	enum class SettingsInterfaceMFDNDMode : uint8_t {
		arcHeadingUp,
		mapHeadingUp,
		mapNorthUp,

		last = mapNorthUp
	};

	class SettingsInterfaceMFDND {
		public:
			bool visible = true;
			SettingsInterfaceMFDNDMode mode = SettingsInterfaceMFDNDMode::arcHeadingUp;
			bool earth = true;
	};

	enum class SettingsInterfaceMFDToolbarMode : uint8_t {
		none,
		main,
		autopilot,
		pressure
	};

	class SettingsInterfaceMFDToolbar {
		public:
			SettingsInterfaceMFDToolbarMode mode = SettingsInterfaceMFDToolbarMode::main;
	};

	class SettingsInterfaceMFD {
		public:
			SettingsInterfaceMFDPFD PFD {};
			SettingsInterfaceMFDND ND {};
			SettingsInterfaceMFDToolbar toolbar {};
			uint8_t splitPercent = 60;

			bool isAnyPanelVisible() const {
				return PFD.visible || ND.visible;
			}
	};

	class SettingsInterfaceDeveloper {
		public:
			bool debugOverlay = false;
	};

	class SettingsInterface : public NVSSerializable {
		public:
			SettingsInterfaceMFD MFD {};
			SettingsInterfaceDeveloper developer {};

		protected:
			const char* getNVSNamespace() override {
				return _namespace;
			}

			void onRead(const NVSStream& stream) override {
				MFD.PFD.visible = stream.getBool(_MFDPFDVisible, true);
				MFD.PFD.FOV = stream.getUint8(_MFDPFDFOV, 50);
				MFD.PFD.flightDirectors = stream.getBool(_MFDPFDFlightDirectors, true);

				MFD.ND.visible = stream.getBool(_MFDNDVisible, true);
				MFD.ND.mode = static_cast<SettingsInterfaceMFDNDMode>(stream.getUint8(_MFDNDMode, static_cast<uint8_t>(SettingsInterfaceMFDNDMode::arcHeadingUp)));
				MFD.ND.earth = stream.getBool(_MFDNDEarth, true);

				MFD.toolbar.mode = static_cast<SettingsInterfaceMFDToolbarMode>(stream.getUint8(_MFDToolbarMode, static_cast<uint8_t>(SettingsInterfaceMFDToolbarMode::main)));

				MFD.splitPercent = stream.getUint8(_MFDSplitPercent, 60);

				developer.debugOverlay = stream.getBool(_developerDebugOverlay, false);
			}

			void onWrite(const NVSStream& stream) override {
				stream.setBool(_MFDPFDVisible, MFD.PFD.visible);
				stream.setUint8(_MFDPFDFOV, MFD.PFD.FOV);
				stream.setBool(_MFDPFDFlightDirectors, MFD.PFD.flightDirectors);

				stream.setBool(_MFDNDVisible, MFD.ND.visible);
				stream.getUint8(_MFDNDMode, static_cast<uint8_t>(MFD.ND.mode));
				stream.setBool(_MFDNDEarth, MFD.ND.earth);

				stream.getUint8(_MFDToolbarMode, static_cast<uint8_t>(MFD.toolbar.mode));

				stream.setUint8(_MFDSplitPercent, MFD.splitPercent);

				stream.setBool(_developerDebugOverlay, developer.debugOverlay);
			}

		private:
			constexpr static auto _namespace = "ifns";
			constexpr static auto _MFDPFDVisible = "ifmpvi";
			constexpr static auto _MFDPFDFOV = "ifmpfo";
			constexpr static auto _MFDPFDFlightDirectors = "ifmpfd";
			constexpr static auto _MFDNDVisible = "ifmnvi";
			constexpr static auto _MFDNDMode = "ifmnmd";
			constexpr static auto _MFDNDEarth = "ifmnea";
			constexpr static auto _MFDToolbarMode = "ifmtmd";
			constexpr static auto _MFDSplitPercent = "ifmsp";
			constexpr static auto _developerDebugOverlay = "ifddo";
	};

	class Settings {
		public:
			SettingsControls controls {};
			SettingsAxis axis {};
			SettingsAutopilot autopilot {};
			SettingsInterface interface {};

			void readAll() {
				controls.read();
				axis.read();
				autopilot.read();
				interface.read();
			}
	};
}

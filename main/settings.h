#pragma once

#include <cstdint>
#include <esp_timer.h>
#include <esp_log.h>
#include <memory>
#include <cstring>

#include "hardware/NVS.h"

namespace pizda {
	class SettingsControls {
		public:
			uint8_t throttle = 0;

			uint32_t referencePressurePa = 0;
			bool referencePressureSTD = false;

			uint32_t minimumAltitudeFt = 0;
			bool minimumAltitudeEnabled = false;

			bool landingGear = false;
			bool strobeLights = false;
	};

	class SettingsAxisData {
		public:
			uint16_t from = 0;
			uint16_t to = 0;
			bool inverted = false;
	};

	class SettingsAxis {
		public:
			SettingsAxisData leverLeft {};
			SettingsAxisData leverRight {};
			SettingsAxisData joystickHorizontal  {};
			SettingsAxisData joystickVertical {};
			SettingsAxisData ring {};
			uint16_t lowPassFactor = 0;
			uint8_t jitteringCutoffValue = 0;
	};

	class SettingsAutopilot {
		public:
			uint16_t speedKt = 0;
			bool autoThrottle = false;

			uint16_t headingDeg = 0;
			bool headingHold = false;

			uint16_t altitudeFt = 0;
			bool levelChange = false;
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

	class SettingsInterface {
		public:
			SettingsInterfaceMFD MFD {};
			SettingsInterfaceDeveloper developer {};
	};

	class Settings {
		public:
			SettingsControls controls {};
			SettingsAxis axis {};
			SettingsAutopilot autopilot {};
			SettingsInterface interface {};

			void read() {
				NVS NVS {};
				NVS.openForReading(Keys::root);

				// -------------------------------- Controls --------------------------------

				controls.throttle = NVS.getFloat(Keys::controlsThrottle, 0);

				controls.referencePressurePa = NVS.getUint32(Keys::controlsReferencePressurePa, 101325);
				controls.referencePressureSTD = NVS.getBool(Keys::controlsReferencePressureSTD, false);

				controls.minimumAltitudeFt = NVS.getUint32(Keys::controlsMinimumAltitudeFt, 350);
				controls.minimumAltitudeEnabled = NVS.getBool(Keys::controlsMinimumAltitudeEnabled, true);

				controls.landingGear = NVS.getBool(Keys::controlsLandingGear, true);
				controls.strobeLights = NVS.getBool(Keys::controlsStrobeLights, true);

				// -------------------------------- Axis --------------------------------

				axis.leverLeft.from = NVS.getUint16(Keys::axisLeverLeftFrom, 0);
				axis.leverLeft.to = NVS.getUint16(Keys::axisLeverLeftTo, 4096);
				axis.leverLeft.inverted = NVS.getUint16(Keys::axisLeverLeftInverted, false);

				axis.leverRight.from = NVS.getUint16(Keys::axisLeverRightFrom, 0);
				axis.leverRight.to = NVS.getUint16(Keys::axisLeverRightTo, 4096);
				axis.leverRight.inverted = NVS.getUint16(Keys::axisLeverRightInverted, false);

				axis.joystickHorizontal.from = NVS.getUint16(Keys::axisJoystickHorizontalFrom, 778);
				axis.joystickHorizontal.to = NVS.getUint16(Keys::axisJoystickHorizontalTo, 2744);
				axis.joystickHorizontal.inverted = NVS.getUint16(Keys::axisJoystickHorizontalInverted, true);

				axis.joystickVertical.from = NVS.getUint16(Keys::axisJoystickVerticalFrom, 1474);
				axis.joystickVertical.to = NVS.getUint16(Keys::axisJoystickVerticalTo, 3031);
				axis.joystickVertical.inverted = NVS.getUint16(Keys::axisJoystickVerticalInverted, false);

				axis.ring.from = NVS.getUint16(Keys::axisRingFrom, 0);
				axis.ring.to = NVS.getUint16(Keys::axisRingTo, 3768);
				axis.ring.inverted = NVS.getUint16(Keys::axisRingInverted, true);

				axis.lowPassFactor = NVS.getUint16(Keys::axisLowPassFactor, 0xFFFF * 75 / 100);
				axis.jitteringCutoffValue = NVS.getUint8(Keys::axisJitteringCutoffValue, 30);

				// -------------------------------- Autopilot --------------------------------

				autopilot.speedKt = NVS.getUint16(Keys::autopilotSpeedKt, 90);
				autopilot.autoThrottle = NVS.getBool(Keys::autopilotAutoThrottle, false);

				autopilot.headingDeg = NVS.getUint16(Keys::autopilotHeadingDeg, 0);
				autopilot.headingHold = NVS.getBool(Keys::autopilotHeadingHold, false);

				autopilot.altitudeFt = NVS.getUint16(Keys::autopilotAltitudeFt, 100);
				autopilot.levelChange = NVS.getBool(Keys::autopilotLevelChange, false);

				// -------------------------------- Interface --------------------------------

				// MFD -> PFD
				interface.MFD.PFD.visible = NVS.getBool(Keys::interfaceMFDPFDVisible, true);
				interface.MFD.PFD.FOV = NVS.getUint8(Keys::interfaceMFDPFDFOV, 50);
				interface.MFD.PFD.flightDirectors = NVS.getBool(Keys::interfaceMFDPFDFlightDirectors, true);

				// MFD -> ND
				interface.MFD.ND.visible = NVS.getBool(Keys::interfaceMFDNDVisible, true);
				interface.MFD.ND.mode = static_cast<SettingsInterfaceMFDNDMode>(NVS.getUint8(Keys::interfaceMFDNDMode, static_cast<uint8_t>(SettingsInterfaceMFDNDMode::arcHeadingUp)));
				interface.MFD.ND.earth = NVS.getBool(Keys::interfaceMFDNDEarth, true);

				// MFD -> toolbar
				interface.MFD.toolbar.mode = static_cast<SettingsInterfaceMFDToolbarMode>(NVS.getUint8(Keys::interfaceMFDToolbarMode, static_cast<uint8_t>(SettingsInterfaceMFDToolbarMode::main)));

				// MFD
				interface.MFD.splitPercent = NVS.getUint8(Keys::interfaceMFDSplitPercent, 60);

				// Developer
				interface.developer.debugOverlay = NVS.getBool(Keys::interfaceDeveloperDebugOverlay, false);

				NVS.close();
			}

			void write() const {
				ESP_LOGI("Settings", "Writing");

				NVS NVS {};
				NVS.openForWriting(Keys::root);

				// -------------------------------- Controls --------------------------------

				NVS.setFloat(Keys::controlsThrottle, controls.throttle);

				NVS.setUint32(Keys::controlsReferencePressurePa, controls.referencePressurePa);
				NVS.setBool(Keys::controlsReferencePressureSTD, controls.referencePressureSTD);

				NVS.setUint32(Keys::controlsMinimumAltitudeFt, controls.minimumAltitudeFt);
				NVS.setBool(Keys::controlsMinimumAltitudeEnabled, controls.minimumAltitudeEnabled);

				NVS.setBool(Keys::controlsLandingGear, controls.landingGear);
				NVS.setBool(Keys::controlsStrobeLights, controls.strobeLights);

				// -------------------------------- Axis --------------------------------

				NVS.setUint16(Keys::axisLeverLeftFrom, axis.leverLeft.from);
				NVS.setUint16(Keys::axisLeverLeftTo, axis.leverLeft.to);
				NVS.setUint16(Keys::axisLeverLeftInverted, axis.leverLeft.inverted);

				NVS.setUint16(Keys::axisLeverRightFrom, axis.leverRight.from);
				NVS.setUint16(Keys::axisLeverRightTo, axis.leverRight.to);
				NVS.setUint16(Keys::axisLeverRightInverted, axis.leverRight.inverted);

				NVS.setUint16(Keys::axisJoystickHorizontalFrom, axis.joystickHorizontal.from);
				NVS.setUint16(Keys::axisJoystickHorizontalTo, axis.joystickHorizontal.to);
				NVS.setUint16(Keys::axisJoystickHorizontalInverted, axis.joystickHorizontal.inverted);

				NVS.setUint16(Keys::axisJoystickVerticalFrom, axis.joystickVertical.from);
				NVS.setUint16(Keys::axisJoystickVerticalTo, axis.joystickVertical.to);
				NVS.setUint16(Keys::axisJoystickVerticalInverted, axis.joystickVertical.inverted);

				NVS.setUint16(Keys::axisRingFrom, axis.ring.from);
				NVS.setUint16(Keys::axisRingTo, axis.ring.to);
				NVS.setUint16(Keys::axisRingInverted, axis.ring.inverted);

				NVS.setUint16(Keys::axisLowPassFactor, axis.lowPassFactor);
				NVS.setUint8(Keys::axisJitteringCutoffValue, axis.jitteringCutoffValue);

				// -------------------------------- Autopilot --------------------------------

				NVS.setUint16(Keys::autopilotSpeedKt, autopilot.speedKt);
				NVS.setBool(Keys::autopilotAutoThrottle, autopilot.autoThrottle);

				NVS.setUint16(Keys::autopilotHeadingDeg, autopilot.headingDeg);
				NVS.setBool(Keys::autopilotHeadingHold, autopilot.headingHold);

				NVS.setUint16(Keys::autopilotAltitudeFt, autopilot.altitudeFt);
				NVS.setBool(Keys::autopilotLevelChange, autopilot.levelChange);

				// -------------------------------- Interface --------------------------------

				// MFD -> PFD
				NVS.setBool(Keys::interfaceMFDPFDVisible, interface.MFD.PFD.visible);
				NVS.setUint8(Keys::interfaceMFDPFDFOV, interface.MFD.PFD.FOV);
				NVS.setBool(Keys::interfaceMFDPFDFlightDirectors, interface.MFD.PFD.flightDirectors);

				// MFD -> ND
				NVS.setBool(Keys::interfaceMFDNDVisible, interface.MFD.ND.visible);
				NVS.getUint8(Keys::interfaceMFDNDMode, static_cast<uint8_t>(interface.MFD.ND.mode));
				NVS.setBool(Keys::interfaceMFDNDEarth, interface.MFD.ND.earth);

				// MFD -> toolbar
				NVS.getUint8(Keys::interfaceMFDToolbarMode, static_cast<uint8_t>(interface.MFD.toolbar.mode));

				// MFD
				NVS.setUint8(Keys::interfaceMFDSplitPercent, interface.MFD.splitPercent);

				// Developer
				NVS.setBool(Keys::interfaceDeveloperDebugOverlay, interface.developer.debugOverlay);

				NVS.commit();
				NVS.close();
			}

			void enqueueWrite() {
				_timeToWrite = esp_timer_get_time() + _writeDelay;
			}

			void tick() {
				if (_timeToWrite == 0 || esp_timer_get_time() < _timeToWrite)
					return;

				_timeToWrite = 0;

				write();
			}

		private:
			constexpr static uint32_t _writeDelay = 2500000;
			uint32_t _timeToWrite = 0;

			class Keys {
				public:
					constexpr static auto root = "st";

					constexpr static auto controlsThrottle = "ctth";
					constexpr static auto controlsReferencePressurePa = "ctrp";
					constexpr static auto controlsReferencePressureSTD = "ctrs";
					constexpr static auto controlsMinimumAltitudeFt = "ctma";
					constexpr static auto controlsMinimumAltitudeEnabled = "ctme";
					constexpr static auto controlsLandingGear = "ctlg";
					constexpr static auto controlsStrobeLights = "ctsl";

					constexpr static auto axisLeverLeftFrom = "axllf";
					constexpr static auto axisLeverLeftTo = "axllt";
					constexpr static auto axisLeverLeftInverted = "axlli";
					constexpr static auto axisLeverRightFrom = "axlrf";
					constexpr static auto axisLeverRightTo = "axlrt";
					constexpr static auto axisLeverRightInverted = "axlri";
					constexpr static auto axisJoystickHorizontalFrom = "axjhf";
					constexpr static auto axisJoystickHorizontalTo = "axjht";
					constexpr static auto axisJoystickHorizontalInverted = "axjhi";
					constexpr static auto axisJoystickVerticalFrom = "axjvf";
					constexpr static auto axisJoystickVerticalTo = "axjvt";
					constexpr static auto axisJoystickVerticalInverted = "axjvi";
					constexpr static auto axisRingFrom = "axrgf";
					constexpr static auto axisRingTo = "axrgt";
					constexpr static auto axisRingInverted = "axrgi";
					constexpr static auto axisLowPassFactor = "axlpf";
					constexpr static auto axisJitteringCutoffValue = "axjc";

					constexpr static auto autopilotSpeedKt = "apsp";
					constexpr static auto autopilotAutoThrottle = "apat";
					constexpr static auto autopilotHeadingDeg = "aphd";
					constexpr static auto autopilotHeadingHold = "aphh";
					constexpr static auto autopilotAltitudeFt = "apal";
					constexpr static auto autopilotLevelChange = "aplc";

					constexpr static auto interfaceMFDPFDVisible = "ifmpvi";
					constexpr static auto interfaceMFDPFDFOV = "ifmpfo";
					constexpr static auto interfaceMFDPFDFlightDirectors = "ifmpfd";
					constexpr static auto interfaceMFDNDVisible = "ifmnvi";
					constexpr static auto interfaceMFDNDMode = "ifmnmd";
					constexpr static auto interfaceMFDNDEarth = "ifmnea";
					constexpr static auto interfaceMFDToolbarMode = "ifmtmd";
					constexpr static auto interfaceMFDSplitPercent = "ifmsp";
					constexpr static auto interfaceDeveloperDebugOverlay = "ifddo";
			};
	};
}

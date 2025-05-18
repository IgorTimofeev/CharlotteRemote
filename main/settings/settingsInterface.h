#pragma once

#include <cstdint>

#include <hardware/NVS/NVSStream.h>
#include <hardware/NVS/NVSSerializable.h>

namespace pizda {
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
				stream.setUint8(_MFDNDMode, static_cast<uint8_t>(MFD.ND.mode));

				stream.setBool(_MFDNDEarth, MFD.ND.earth);

				stream.setUint8(_MFDToolbarMode, static_cast<uint8_t>(MFD.toolbar.mode));

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
}

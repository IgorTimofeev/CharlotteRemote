#pragma once

#include <cstdint>

#include <NVSSettings.h>

namespace pizda {
	using namespace YOBA;
	
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
		autopilot,
		baro,
		trim,
		lights,
		log
	};

	class SettingsInterfaceMFDToolbar {
		public:
			SettingsInterfaceMFDToolbarMode mode = SettingsInterfaceMFDToolbarMode::none;
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

	class SettingsInterface : public NVSSettings {
		public:
			SettingsInterfaceMFD MFD {};
			SettingsInterfaceDeveloper developer {};

		protected:
			const char* getNamespace() override {
				return _namespace;
			}

			void onRead(const NVSStream& stream) override {
				MFD.PFD.visible = stream.readBool(_MFDPFDVisible, true);
				MFD.PFD.FOV = stream.readUint8(_MFDPFDFOV, 50);
				MFD.PFD.flightDirectors = stream.readBool(_MFDPFDFlightDirectors, true);

				MFD.ND.visible = stream.readBool(_MFDNDVisible, true);
				MFD.ND.mode = static_cast<SettingsInterfaceMFDNDMode>(stream.readUint8(_MFDNDMode, static_cast<uint8_t>(SettingsInterfaceMFDNDMode::arcHeadingUp)));

				MFD.ND.earth = stream.readBool(_MFDNDEarth, true);

				MFD.toolbar.mode = static_cast<SettingsInterfaceMFDToolbarMode>(stream.readUint8(_MFDToolbarMode, static_cast<uint8_t>(SettingsInterfaceMFDToolbarMode::none)));
				
				MFD.splitPercent = stream.readUint8(_MFDSplitPercent, 60);

				developer.debugOverlay = stream.readBool(_developerDebugOverlay, false);
			}

			void onWrite(const NVSStream& stream) override {
				stream.writeBool(_MFDPFDVisible, MFD.PFD.visible);
				stream.writeUint8(_MFDPFDFOV, MFD.PFD.FOV);
				stream.writeBool(_MFDPFDFlightDirectors, MFD.PFD.flightDirectors);

				stream.writeBool(_MFDNDVisible, MFD.ND.visible);
				stream.writeUint8(_MFDNDMode, static_cast<uint8_t>(MFD.ND.mode));

				stream.writeBool(_MFDNDEarth, MFD.ND.earth);

				stream.writeUint8(_MFDToolbarMode, static_cast<uint8_t>(MFD.toolbar.mode));

				stream.writeUint8(_MFDSplitPercent, MFD.splitPercent);

				stream.writeBool(_developerDebugOverlay, developer.debugOverlay);
			}

		private:
			constexpr static auto _namespace = "if";
			constexpr static auto _MFDPFDVisible = "mpvi";
			constexpr static auto _MFDPFDFOV = "mpfo";
			constexpr static auto _MFDPFDFlightDirectors = "mpfd";
			constexpr static auto _MFDNDVisible = "mnvi";
			constexpr static auto _MFDNDMode = "mnmd";
			constexpr static auto _MFDNDEarth = "mnea";
			constexpr static auto _MFDToolbarMode = "mtmd";
			constexpr static auto _MFDSplitPercent = "msp";
			constexpr static auto _developerDebugOverlay = "ddo";
	};
}
